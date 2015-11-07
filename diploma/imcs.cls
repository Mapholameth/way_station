%\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{imcs}[2013/05/24 IMCS diploma report style]
\LoadClass[14pt,a4paper]{extarticle}
\RequirePackage{fancyhdr}

\usepackage{ifxetex}

\ifxetex
  \usepackage{fontspec}
  \setmainfont[Ligatures=TeX]{Times New Roman}
  \setsansfont{Arial}
  \setmonofont[BoldFont=*,BoldFeatures={FakeBold=1.4},
    ItalicFont=*,ItalicFeatures={FakeSlant=0.2},
    BoldItalicFont=*,BoldItalicFeatures={FakeBold=1.4,FakeSlant=0.2}
    ]{Consolas}
\else
  \usepackage[T1,T2A]{fontenc}
  \usepackage[utf8]{inputenc}
\fi

\usepackage[english,russian]{babel}

\RequirePackage{indentfirst}
\RequirePackage[unicode,
  bookmarksnumbered,
  pdfstartview=FitH,
  pdfpagelayout=OneColumn,
  colorlinks=true,
  linkcolor=blue,
  plainpages=false,
  breaklinks=true
]{hyperref}
\RequirePackage{graphicx}
\DeclareGraphicsExtensions{.pdf,.png}
\graphicspath{{./images/}}
\RequirePackage{scalefnt}
\RequirePackage{geometry}

\geometry{left=30mm,top=20mm,bottom=20mm,right=10mm}
\parindent=1.25cm

\fancypagestyle{plain}{%
\fancyhf{} % clear all header and footer fields
\fancyfoot[R]{\thepage} % except the right one
\renewcommand{\headrulewidth}{0pt}
\renewcommand{\footrulewidth}{0pt}}
\pagestyle{plain}

\RequirePackage{setspace}
\onehalfspacing

\renewcommand\thesection{\thechapter\@arabic\c@section.}
\renewcommand\thesubsection{\thesection\@arabic\c@subsection.}
\renewcommand\thesubsubsection{\thesubsection\@arabic\c@subsubsection.}

\DeclareOption*{% pass all extra options to report class (for instance - draft)
    \PassOptionsToClass{\CurrentOption}{article}%
}
\def\groupname#1{\gdef\@groupname{#1}}
\def\mentorname#1{\gdef\@mentorname{#1}}
\def\mentorinfo#1{\gdef\@mentorinfo{#1}}
\def\year#1{\gdef\@year{#1}}

\newcommand{\sectionline}{
  \nointerlineskip \vspace{0.25\baselineskip}
  \hspace{\fill}\rule{\linewidth}{2pt}\hspace{\fill}
  \par\nointerlineskip \vspace{\baselineskip}
}

\newcommand{\superunder}[2]{
    {\tiny\lower\baselineskip\vbox{\halign{##\cr\normalsize #1 \tiny\cr #2 \cr}}}
}

\ProcessOptions\relax
