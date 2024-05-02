#ifndef PMU_PERF_HPP__
#define PMU_PERF_HPP__

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE
#endif

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <linux/perf_event.h>
#include <sys/syscall.h>
#include <vector>
#include <iostream>

namespace dev_ctl
{

typedef struct
{
    uint32_t type;
    uint32_t perf_id;
} event_t;

union EventsReadFormat
{
    char buf[4096];
    struct ReadFormat
    {
        uint64_t nr;
        uint64_t time_enabled;
        uint64_t time_running;
        struct
        {
            uint64_t value;
            uint64_t id;
        } values[];
    } rf;
};

struct Event
{
    int fd = -1;
    event_t event;
    uint64_t id;

    Event(const pid_t pid, const event_t event_info, const int group_fd)
    {
        int saved_errno = errno;

        event = event_info;

        struct perf_event_attr hw_event;
        memset(&hw_event, 0, sizeof(struct perf_event_attr));
        hw_event.type           = event_info.type;
        hw_event.size           = sizeof(struct perf_event_attr);
        hw_event.config         = event_info.perf_id;
        hw_event.exclude_kernel = 1;
        hw_event.disabled       = 1;
        hw_event.exclude_hv     = 1;
        hw_event.read_format    = PERF_FORMAT_GROUP              | PERF_FORMAT_ID |
                                  PERF_FORMAT_TOTAL_TIME_RUNNING | PERF_FORMAT_TOTAL_TIME_ENABLED;

        fd = syscall(__NR_perf_event_open, &hw_event, pid, -1, group_fd, 0);
        if (fd == -1) {
            std::cerr << "syscall(__NR_perf_event_open, ...) returned -1: " <<
                strerror(errno) << std::endl;
        }

        if (ioctl(fd, PERF_EVENT_IOC_ID, &id) == -1) {
            std::cerr << "ioctl() returned -1: " << strerror(errno) << std::endl;
        }

        if (errno == 0)
            errno = saved_errno;
    }

    ~Event()
    {
        disable_collection();
    }

    Event(const Event &event_cp)
    {
        id = event_cp.id;
        fd = event_cp.fd;
        event = event_cp.event;
    }

    Event &operator=(Event &&event_cp)
    {
        id = event_cp.id;
        fd = event_cp.fd;
        event = event_cp.event;
        event_cp.fd = -1;

        return *this;
    }

    int enable_collection() const
    {
        return ioctl(fd, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);
    }

    int disable_collection(bool close_event = false) const
    {
        int error_status = ioctl(fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
        if (close_event == true)
            close(fd);

        return error_status;
    }

};

class Events
{
    pid_t pid = -1;
    std::vector<Event> events;

public:
    Events(const pid_t pid, const std::vector<event_t> events_info)
    {
        add_events(pid, events_info);
    }

    Events() {}

    Events(const Events &event)       = delete;
    Events &operator=(Events &&event) = delete;

    ~Events() {}

    void add_events(const pid_t pid_proc, const std::vector<event_t> events_info)
    {
        pid = pid_proc;
        events.push_back(Event(pid, events_info[0], -1)); // PMU leader

        for (size_t i = 1; i < events_info.size(); ++i)
        {
            events.push_back(Event(pid, events_info[i], events[0].fd));
        }

        ioctl(events[0].fd, PERF_EVENT_IOC_RESET,   PERF_IOC_FLAG_GROUP);
        ioctl(events[0].fd, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
    }

    void clear_events()
    {
        events.clear();
    }

    size_t get_num_events() const
    {
        return events.size();
    }

    int enable_collection() const
    {
        return events[0].enable_collection();
    }

    int disable_collection(bool close_events = false) const
    {
        if (close_events == true)
        {
            int error_status = 0;
            for (auto event: events)
            {
                error_status += event.disable_collection(close_events);
            }

            return error_status;
        }
        else
            return events[0].disable_collection(close_events);
    }

    std::vector<float> read_events() const
    {
        EventsReadFormat reader = {};

        ssize_t n_read_bytes = read(events[0].fd, &reader, sizeof(reader));
        if (n_read_bytes <= 0)
        {
            return std::vector<float>(0);
        }

        std::vector<float> values;

        for (auto event: events)
        {
            for (size_t i = 0; i < reader.rf.nr; i++)
            {
                if (reader.rf.values[i].id == event.id)
                {
                    values.push_back((float) reader.rf.values[i].value);
                    break;
                }
            }
        }

        values.push_back((float) reader.rf.time_running);
        values.push_back((float) reader.rf.time_enabled);

        enable_collection();

        return values;
    }

};

}

#endif // PMU_PERF_HPP__
