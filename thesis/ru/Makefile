# .PHONY: clean

all: compile_all clean

compile_all: bibtex_compile pres_compile thesis_compile

pres_compile:
	pdflatex --shell-escape pres.tex
thesis_compile:
	pdflatex --shell-escape thesis.tex
bibtex_compile: pres_compile thesis_compile
	bibtex thesis

clean:
	$(RM) -rf *.aux *.fdb_latexmk *.fls *.out *.gz *.log *.bbl \
	       *.blg *.toc *.xml *.nav *.bcf *.snm
