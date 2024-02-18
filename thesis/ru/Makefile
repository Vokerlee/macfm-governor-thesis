all:
	pdflatex --shell-escape main.tex
	bibtex main
	pdflatex --schell-escape main.tex

clean:
	rm -rf *.aux *.fdb_latexmk *.fls *.out *.gz *.log *.bbl *.blg *.toc
