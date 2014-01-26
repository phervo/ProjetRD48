%=============================================================================
% Classe report-rd-info (basée sur la classe standard ``report'').
% Rapport de travail de recherche et développement du département d'informatique de Polytech Nantes
% Auteur : José Martinez
%=============================================================================
\def\REPORT@RD@INFO@Version{2.6}
% 1.0
% - version initiale
% 1.1
% - nouveaux en-têtes de chapitres
% - prise en compte de deux auteurs
% - correction de la largeur du résumé dans la version << écran >> du rapport
% - ajout des fiches de suivi
% 2.0
% - protection des identifiants internes avec des << @ >>
% - ajout du tableau récapitulatif du temps consacré au projet
% - traduction en anglais des parties du document auto-générées
% 2.1
% - conditionnelle \ifscreen renommée pour être rendue accessible dans la rédaction
%   d'un document (par exemple, pour choisir l'orientation des figures)
% - ajout d'un avertissement en verso de première de couverture
% 2.2
% - prise en compte de deux encadrants (les rares suivants sont à ajouter << manuellement >>)
% - modification de la mise en page de la page de titre
% 2.3
% - couverture sur une seule page pour la version << écran >>
% - correction d'une bogue lors du passage à une seule page de titre pour
%   la version << écran >>
% 2.4
% - prise en compte d'UTF-8 (et autres codages) pour les textes générés par cette classe
% - suppression des tirets longs lorsque l'équipe est absente
% - traduction en anglais de l'avertissement sur le << copyright >>
% - ajout de la dernière ligne verticale dans le tableau des heures lorsqu'il y a un
%   seul élève ingénieur
% - remplacements de sauts fixes par des ressorts dans la page de titre
% - possibilité de décrire un partenaire hors laboratoires Polytech et d'y associer son logo
% - indications supplémentaires dans le modèle de rapport
% - ajout de cet historique des modifications
% 2.5
% - modification des marges sur la version écran pour éviter de (trop) écraser le logo Polytech
% - modification de la couleur des chapitres pour celle de Polytech Nantes
% 2.6
% - logo Polytech vertical en version écran sur la page de garde
% - suppression de l'espacement avant un titre de chapitre et entre le numéro et le titre dans la version écran, ainsi que réduction de l'espacement avant le texte
% - macro \ifenglish rendue visible pour changer le titre de la liste des algorithmes dans le document principal
% - chapitres étoilés mis en forme de manière similaire au chapitres (couleur des caractère et espacements)
% - modification du style des parties en phase avec celui des chapitres.
% - ajout du coordinateur
% - refonte du code
% - modèle de rapport en version anglaise, bien plus court
% - suppression de "version X.Y du " dans le modèle car plusieurs élèves le répétaient sans suivre la consigne de ne pas le faire !...
% - référence en lignes de la bibliographie mises en notes de bas de page
\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{report-rd-info}[2013/07/16 report-rd-info version \REPORT@RD@INFO@Version]
\typeout{(c) 2008-2013 Jos\'e Martinez, \'Ecole polytechnique de l'universit\'e de Nantes}
\typeout{ }

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Gestion des options =
% Déclaration des options possibles
\newif\ifenglish
\DeclareOption{english}{\englishtrue}
\DeclareOption{francais}{\englishfalse}

\newif\if@inDraftMode
\DeclareOption{draft}{\@inDraftModetrue}
\DeclareOption{final}{\@inDraftModefalse}

\DeclareOption{10pt}{\gdef\@pt@size{10pt}}
\DeclareOption{11pt}{\gdef\@pt@size{11pt}}
\DeclareOption{12pt}{\gdef\@pt@size{12pt}}

\newif\ifscreen
\screenfalse
\DeclareOption{screen}{\screentrue}

\ExecuteOptions{francais,final,11pt}
\ProcessOptions\relax

%<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Fin gestion des options =

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Chargement de paquetages additionnels =

\if@inDraftMode
   \ifscreen
      \PassOptionsToClass{\@pt@size,oneside,twocolumn,draft}{report}
   \else
      \PassOptionsToClass{\@pt@size,twoside,draft}{report}
   \fi
\else
   \ifscreen
      \PassOptionsToClass{\@pt@size,oneside,twocolumn,final}{report}
   \else
      \PassOptionsToClass{\@pt@size,twoside,final}{report}
   \fi
\fi
\LoadClass{report}

% Attention : ne pas déplacer l'appel pour charger 'color'
\RequirePackage{color}
%\definecolor{ch@ptercolor}{rgb}{0.85,0.0,0.0} % première version : rouge
%\definecolor{ch@ptercolor}{rgb}{0.035,0.259,0.439}% bleu foncé du logo Polytech
\definecolor{ch@ptercolor}{rgb}{0,0.678,0.937}% bleu clair du logo Polytech
\definecolor{r@tiomaxcolor}{rgb}{0.5,0.5,0.5}
\RequirePackage{graphicx}
\RequirePackage{times}
\ifscreen
   \RequirePackage[screen, columnsep=2.5em, margin=1.5cm, rmargin=2.5cm]{geometry}
\else
   \RequirePackage{a4wide}
%    \RequirePackage{fancyhdr}
%       \pagestyle{fancy}
\fi
\RequirePackage{ifthen}
\RequirePackage[bookmarks,%
                colorlinks,%
                urlcolor=blue,%
                citecolor=blue,%
                linkcolor=blue,%
                hyperfigures,%
                pagebackref,%
                pdfcreator=LaTeX,%
                breaklinks=true,%
                pdfpagelayout=SinglePage,%
                bookmarksopen=true,%
                bookmarksopenlevel=1]{hyperref}

\RequirePackage[english,francais]{babel}
\ifenglish
\AtBeginDocument{%
  \selectlanguage{english}
}
\else
\AtBeginDocument{%
  \selectlanguage{francais}
}
\fi

\RequirePackage{multirow}
\RequirePackage{calc}

%<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< fin chargement de paquetages additionnels =

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Localisation
\ifenglish
	\newcommand{\acknowledgementsname}{Acknowledgements}
	\newcommand{\rapportname}{Polytechnic School of the University of Nantes\par Computer Science Department\\[1em] Research and Development Report}
	\newcommand{\supervisedname}{supervised by}
   \newcommand{\coordinatorname}{coordinator: }
	\AtBeginDocument{%
		\def\proofname{Proof}
		\def\today{\ifcase\month\or
		January\or February\or March\or April\or May\or June\or
		July\or August\or September\or October\or November\or December\fi
		~\number\year}
		\def\abstractname{Abstract}
		\newcommand{\surname}[1]{#1}
		\renewcommand{\refname}{References}
		\newcommand{\categoryname}{Categories and Subject Descriptors:}
		\newcommand{\termsname}{General Terms:}
		\newcommand{\keywordsname}{Additional Key Words and Phrases:}}

	\newcommand{\BibNameFmt}{\rm}
	\newcommand{\EndBibNameFmt}{\rm}
	\newcommand{\andMacro}{and}
	\newcommand{\editorsMacro}{eds.}
	\newcommand{\editorMacro}{ed.}
	\newcommand{\editionMacro}{edition}
	\newcommand{\chapterMacro}{chapter}
	\newcommand{\trMacro}{Technical report}
	\newcommand{\ofMacro}{of}
	\newcommand{\thesisMacro}{Doctoral thesis}
	\newcommand{\toappearMacro}{To appear}

	\newcommand{\janMacro}{January }
	\newcommand{\febMacro}{February }
	\newcommand{\marMacro}{March }
	\newcommand{\aprMacro}{April }
	\newcommand{\mayMacro}{May }
	\newcommand{\junMacro}{June }
	\newcommand{\julMacro}{July }
	\newcommand{\augMacro}{August }
	\newcommand{\sepMacro}{September }
	\newcommand{\octMacro}{October }
	\newcommand{\novMacro}{November }
	\newcommand{\decMacro}{December }

	\newcommand{\WeeklyReport}{Weekly Report \#}
	\newcommand{\WeeklyReportFrom}{From}
	\newcommand{\WeeklyReportTo}{to}
	\newcommand{\WeeklyAuthorWork}{Working time of}
	\newcommand{\WeeklyWorkDone}{Work done}
	\newcommand{\WeeklyWorkNotDone}{Work not yet done}
	\newcommand{\WeeklyExchange}{Communication with the client}
	\newcommand{\WeeklyNextPlanning}{Next week planning}
\else
	\newcommand{\acknowledgementsname}{Remerciements}
	\def\proofname{D\'emonstration}
	\newcommand{\rapportname}{\'Ecole polytechnique de l'universit\'e de Nantes\par D\'epartement d'informatique\\[1em] Rapport de recherche et d\'eveloppement}
	\newcommand{\supervisedname}{encadr\'e par}
   \newcommand{\coordinatorname}{coordinateur : }

	\AtBeginDocument{%
	\def\today{\ifcase\month\or
	 janvier\or f\'evrier\or mars\or avril\or mai\or juin\or
	 juillet\or ao\^ut\or septembre\or octobre\or novembre\or d\'ecembre\fi
	 ~\number\year}
	\def\abstractname{R\'esum\'e}
	\renewcommand{\refname}{R\'ef\'erences}
	\newcommand{\surname}[1]{\textsc{#1}}
	\newcommand{\categoryname}{Cat\'egories et descripteurs de sujets~:}
	\newcommand{\termsname}{Termes g\'en\'eraux~:}
	\newcommand{\keywordsname}{Mots-cl\'es additionnels et phrases~:}}

	\newcommand{\BibNameFmt}{\sc}
	\newcommand{\EndBibNameFmt}{\rm}
	\newcommand{\andMacro}{et}
	\newcommand{\editorsMacro}{r\'eds.}
	\newcommand{\editorMacro}{r\'ed.}
	\newcommand{\editionMacro}{\'edition}
	\newcommand{\chapterMacro}{chapitre}
	\newcommand{\trMacro}{Rapport technique}
	\newcommand{\ofMacro}{de}
	\newcommand{\thesisMacro}{Th\`ese de doctorat}
	\newcommand{\toappearMacro}{\`A para\^\i{}tre}

	\newcommand{\janMacro}{janvier }
	\newcommand{\febMacro}{f\'evrier }
	\newcommand{\marMacro}{mars }
	\newcommand{\aprMacro}{avril }
	\newcommand{\mayMacro}{mai }
	\newcommand{\junMacro}{juin }
	\newcommand{\julMacro}{juillet }
	\newcommand{\augMacro}{ao\^ut }
	\newcommand{\sepMacro}{septembre }
	\newcommand{\octMacro}{octobre }
	\newcommand{\novMacro}{novembre }
	\newcommand{\decMacro}{d\'ecembre }

	\newcommand{\WeeklyReport}{Fiche de suivi de la semaine~}
	\newcommand{\WeeklyReportFrom}{du}
	\newcommand{\WeeklyReportTo}{au}
	\newcommand{\WeeklyAuthorWork}{Temps de travail de}
	\newcommand{\WeeklyWorkDone}{Travail effectu\'e}
	\newcommand{\WeeklyWorkNotDone}{Travail non effectu\'e}
	\newcommand{\WeeklyExchange}{\'Echanges avec le commanditaire}
	\newcommand{\WeeklyNextPlanning}{Planification pour la semaine prochaine}
\fi
%<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Fin customisations suivant la langue =


%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Champs d'identification =

%%%%%%%
% Titre
%

\global\let\@title\@empty

\renewcommand\title[1]{%
   \ifx\@title\@empty
      \gdef\@title{#1}
   \else
      \gdef\@title{\textcolor{red}{title d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {title d'efini plusieurs fois}
                 {}
   \fi}

%%%%%%%%%%%%
% Sous-titre
%

\global\let\@subtitle\@empty

\newcommand\subtitle[1]{%
   \ifx\@subtitle\@empty
      \gdef\@subtitle{#1}
   \else
      \gdef\@subtitle{\textcolor{red}{subtitle d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {subtitle d'efini plusieurs fois}
                 {}
   \fi}

%%%%%%%%%%
% Auteur A
%

\global\let\@authorA\@empty
\global\let\@authorAfirstname\@empty
\global\let\@authorAlastname\@empty

\newcommand\authorA[2]{%
   \ifx\@authorA\@empty
      \gdef\@authorA{#1 \textsc{#2}}
      \gdef\@authorAfirstname{#1}
      \gdef\@authorAlastname{\textsc{#2}}
   \else
      \gdef\@authorA{\textcolor{red}{authorA d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {authorA d'efini plusieurs fois}
                 {}
   \fi}

%%%%%%%%%%
% Auteur B
%

\global\let\@authorB\@empty
\global\let\@authorBfirstname\@empty
\global\let\@authorBlastname\@empty

\newcommand\authorB[2]{%
   \ifx\@authorA\@empty
      \ClassError{report-rd-info}
                 {authorB d'efini avant authorA}
                 {}
   \else
   \ifx\@authorB\@empty
      \gdef\@authorB{#1 \textsc{#2}}
      \gdef\@authorBfirstname{#1}
      \gdef\@authorBlastname{\textsc{#2}}
   \else
      \gdef\@authorB{\textcolor{red}{authorB d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {authorB d'efini plusieurs fois}
                 {}
   \fi\fi}

%%%%%%%%%%%
% Encadrant
%

\global\let\@supervisor\@empty

\newcommand\supervisor[2]{%
   \ifx\@supervisor\@empty
      \gdef\@supervisor{#1 \textsc{#2}}
   \else
      \gdef\@supervisor{\textcolor{red}{supervisor d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {supervisor d'efini plusieurs fois}
                 {}
   \fi}

%%%%%%%%%%%%%%
% Co-encadrant
%

\global\let\@cosupervisor\@empty

\newcommand\cosupervisor[2]{%
   \ifx\@supervisor\@empty
      \ClassError{report-rd-info}
                 {cosupervisor d'efini avant supervisor}
                 {}
   \else
   \ifx\@cosupervisor\@empty
      \gdef\@cosupervisor{#1 \textsc{#2}}
   \else
      \gdef\@cosupervisor{\textcolor{red}{cosupervisor d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {cosupervisor d'efini plusieurs fois}
                 {}
   \fi\fi}

%%%%%%%%%%%%%%
% Coordinateur
%

\global\let\@coordinator\@empty

\newcommand\coordinator[2]{%
   \ifx\@coordinator\@empty
      \gdef\@coordinator{#1 \textsc{#2}}
   \else
      \gdef\@coordinator{\textcolor{red}{Coordinateur d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {Coordinateur d'efini plusieurs fois}
                 {}
   \fi}

%%%%%%%
% Thème
%

\global\let\@theme\@empty

\newcommand\theme[1]{%
   \ifx\@theme\@empty
      \gdef\@theme{#1}
   \else
      \gdef\@theme{\textcolor{red}{theme d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {theme d'efini plusieurs fois}
                 {}
   \fi}

%%%%%%%%%%%%%
% Institution
%

\global\let\@institution\@empty

\newcommand\institution[1]{%
   \ifx\@institution\@empty
      \gdef\@institution{#1}
   \else
      \gdef\@institution{\textcolor{red}{institution d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {institution d'efini plusieurs fois}
                 {}
   \fi}

%%%%%%%%%%%%%%%%
% Co-institution
%

\global\let\@coinstitution\@empty
\global\let\@coinstitutionlogo\@empty
\global\let\@coinstitutionlogowidth\@empty

\newcommand\coinstitution[3]{%
   \ifx\@institution\@empty
      \ClassError{report-rd-info}
                 {coinstitution d'efinie avant institution}
                 {}
   \else
   \ifx\@coinstitution\@empty\relax
      \gdef\@coinstitution{#1}
      \gdef\@coinstitutionlogo{#2}
      \gdef\@coinstitutionlogowidth{#3}
   \else
      \gdef\@coinstitution{\textcolor{red}{coinstitution d\'efini plusieurs fois}}
      \ClassError{report-rd-info}
                 {coinstitution d'efini plusieurs fois}
                 {}
   \fi\fi}

%<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Fin champs d'identification =

\newcommand{\maketitlelarge}
   {%
   \ifscreen
      \large
   \else
      \Large
   \fi
   }

\newcommand{\maketitleheading}
   {% En-tête
   {%
   \maketitlelarge
   \textsc{\rapportname}
   }}

\newcommand{\maketitletitle}
   {% Titre et sous-titre
   {%
   \usefont{T1}{ptm}{b}{n}\selectfont
   \ifscreen
      \Huge
   \else
      \fontsize{30pt}{32pt}\selectfont
   \fi
   \ifx\@title\@empty
      \textcolor{red}{Titre manquant / \emph{Missing Title}}
   \else
      \@title
   \fi
   \par
   \ifx\@subtitle\@empty
      \relax
   \else
      \vspace{.5em}
      {%
      \usefont{T1}{ptm}{b}{sl}\selectfont
      \ifscreen
         \huge
      \else
         \fontsize{30pt}{32pt}\selectfont
      \fi
      \@subtitle
      \par
      }
   \fi
   }}

\newcommand{\maketitleauthors}
   {% Auteurs
   {%
   \maketitlelarge
   \bfseries
   \ifx\@authorA\@empty
      \textcolor{red}{Auteur absent / \emph{Missing Author}}
   \else
      \@authorA
   \fi
   \ifx\@authorB\@empty
      \relax
   \else
      \ \&\ \@authorB
   \fi
   \par
   }}

\newcommand{\maketitledate}
   {% Date
   {%
   \maketitlelarge
   \bfseries
   \@date
   \par
   }}

\newcommand{\maketitlesupervision}
   {% Encadrement
   {%
   \maketitlelarge
   \ifx\@supervisor\@empty
      \texcolor{red}{Encadrant manquant / \emph{Missing Supervisor}}
   \else
      \supervisedname\ \@supervisor
   \fi
   \ifx\@cosupervisor\@empty
      \relax
   \else
      \ \&\ \@cosupervisor
   \fi
   \par
   }}

\newcommand{\maketitleinstitutions}
   {% Institutions
   \ifx\@institution\@empty
      \relax
   \else
      \ifx\@theme\@empty
         \textcolor{red}{Institution manquante alors que le th\`eme est fourni / \emph{Missing institution with given theme}}
      \else
         \maketitlelarge
         ---~\@theme~---%
         \par
      \fi
   \fi
   \ifx\@institution\@empty
      \relax
   \else
      \ifthenelse{\equal{\@institution}{LINA}}
         {% then
         \maketitlelarge
         \textsc{Laboratoire d'informatique de Nantes-Atlantique}%
         \par
         }
      {% elsif
      \ifthenelse{\equal{\@institution}{IRCCyN}}
         {% then
         \maketitlelarge
         \textsc{Institut de recherche en communications et en cybern\'etique de Nantes}%
         \par
         }
      {% else
         \maketitlelarge
         \textsc{\@institution}%
         \par
         }}
   \fi
   \ifx\@coinstitution\@empty
      \relax
   \else
      \maketitlelarge
      \textsc{\@coinstitution}%
      \par
   \fi
   }

\newcommand{\maketitlecoordination}
   {% Coordination
   \ifx\@coordinator\@empty
   \else
      \coordinatorname \@coordinator
      \par
      \vfill
   \fi
   }

\newcommand{\maketitlefooting}
   {% Pied de page
   \hfill
   \hfill
   \ifx\@institution\@empty
      \relax
   \else
      \ifthenelse{\equal{\@institution}{LINA}}
         {% then
         \includegraphics[height=1.5cm]{logos/LINA}
         }
      {% elsif
      \ifthenelse{\equal{\@institution}{IRCCyN}}
         {% then
         \includegraphics[height=1.5cm]{logos/IRCCyN}
         }
      {% else
         \includegraphics[height=1.5cm]{logos/\@institution}
         }}
   \fi
   \ifx\@coinstitutionlogo\@empty
      \relax
   \else
      \hfill
      \includegraphics[width=\@coinstitutionlogowidth]{logos/\@coinstitutionlogo}
   \fi
   \hfill
   \ifscreen
      \raisebox{0pt}[0pt][0pt]{\includegraphics[height=1.5cm]{logos/UnivNantes}}
   \else
      \includegraphics[height=1.5cm]{logos/UnivNantes}
   \fi
   }

\newcommand{\maketitlecontents}
   {% Mise en page générale
   \centering

   \maketitleheading

   \vfill

   \maketitletitle

   \vfill

   \maketitleauthors

   \vfill

   \maketitledate

   \vfill

   \maketitlesupervision

   \vfill

   \maketitleinstitutions

   \vfill

   \maketitlecoordination

   \vfill

   \maketitlefooting
   }

\renewcommand{\maketitle}
   {%
   \ifscreen
      \begin{titlepage}
         \hspace{-3.5em}\raisebox{-\textheight}[0pt][0pt]{\includegraphics[height=\textheight]{logos/PolytechNantes-vertical}}\hfill~
         \begin{list}{}{\leftmargin=2.5cm}\item

            \maketitlecontents
         \end{list}
      \end{titlepage}
   \else
      \begin{titlepage}
         \hspace{-\marginparwidth}\raisebox{\voffset}[0pt][0pt]{\includegraphics[width=0.5\textwidth]{logos/PolytechNantes}}

         \maketitlecontents
      \end{titlepage}
   \fi

	%% Deuxième page : texte sur les droits d'auteur

	\thispagestyle{empty}

	~

	\vfill

   \ifenglish
      \noindent\textbf{Warning}

      All rights reserved.  No part of this report may be reproduced, stored, published or transmitted by any means and in any form without prior permission.

      Any copy, by xerography, photography, photocopy, film, electronic support, or any other, is an infringement of the copyright.
	\else
      \noindent\textbf{Avertissement}

      Toute reproduction, m\^eme partielle, par quelque proc\'ed\'e que ce soit, est interdite sans autorisation pr\'ealable.

      Une copie par x\'erographie, photographie, photocopie, film, support magn\'etique ou autre, constitue une contrefa\c con passible des peines pr\'evues par la loi.
   \fi

	\clearpage
  % Troisième page : rappel de titre + auteurs + résumé
  %                  + classification + mots-clés
   \begin{center}
      {%
      \huge
      \bfseries
      \@title
      }%
      \par
      \ifx\@subtitle\@empty
         \relax
      \else
        {%
        \vspace{.5em}
        \Large
        \bfseries
        \@subtitle
        \par
        }%
      \fi
      \vspace{0.5cm}
      {
      \@authorA
      \ifx\@authorB\@empty
         \relax
      \else
         \ \&\ \@authorB
      \fi
      }%
      \par
   \end{center}
  \vspace{1cm}
  \unvcopy\@abstractBoxGlobal\par
  \vspace{0.5cm}
  \unvcopy\@classificationBoxGlobal
  \cleardoublepage
%  \pagestyle{fancy}
%  \lhead[]{}
%  \rhead[]{}
%  \lfoot[]{}
%  \rfoot[]{}
%  \cfoot[]{\thepage}

%  \lhead[\thepage]{}
%  \rhead[]{\thepage}
%  \lfoot[]{}
%  \rfoot[]{}
%  \cfoot[]{}
} % \maketitle

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Quatrième de couverture =

%<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Fin quatrième de couverture =

\newsavebox{\@abstractBox}
\newbox{\@abstractBoxGlobal}
\renewenvironment{abstract}{%
  \begin{lrbox}{\@abstractBox}%
    \begin{minipage}{\columnwidth}
    \begin{center}
      \bfseries \abstractname
    \end{center}
    }{\end{minipage}\end{lrbox}%
    \global\setbox\@abstractBoxGlobal=\vbox{\copy\@abstractBox}}

% Catégories ACM
\newsavebox{\@classificationBox}
\newbox{\@classificationBoxGlobal}
\newenvironment{classification}{%
  \begin{lrbox}{\@classificationBox}%
    \begin{minipage}{\columnwidth}}{%
      % La partie ci-dessous est reprise de esub2acm.
            \ifx\@categories\@empty \else
                \vskip 4\p@
                \def\and{\unskip{\rm;} }%
                \categoryname\ \@categories\par
            \fi
            \ifx\@terms\@empty\else
                \vskip 4\p@
                \termsname\ \ignorespaces
                \@terms
                \par
            \fi
            \ifx\@keywords\@empty\else
                \vskip 4\p@
                \keywordsname\ \ignorespaces
                \@keywords
                \par
            \fi
      \end{minipage}\end{lrbox}
    \global\setbox\@classificationBoxGlobal=\vbox{\copy\@classificationBox}}
% Ci-dessous: code repris de esub2acm.cls
\def\terms{\gdef\@terms}
\let\@terms\@empty
\def\keywords{\gdef\@keywords}
\let\@keywords\@empty
\def\category#1#2#3{%
    \@ifnextchar [{\@category{#1}{#2}{#3}}{\@category{#1}{#2}{#3}[]}%
}
\def\@category#1#2#3[#4]{%
    \edef\@tempa{\ifx \@categories\@empty \else ; \fi}%
    \begingroup
        \def\protect{\noexpand\protect\noexpand}%
        \let\and\relax
        \xdef\@categories{%
            \@categories
            \@tempa
            #1 [{\bf #2}]%
            \if!#4!%
                \if!#3!\else : #3\fi
            \else
                :\space
                \if!#3!\else #3\kern\z@---\hskip\z@\fi
                {\it #4}%
            \fi
        }%
    \endgroup
}

\let\@categories\@empty


\def\cleardoublepage{\clearpage\thispagestyle{empty}%
  \ifodd\c@page\else
    \hbox{}\newpage\thispagestyle{empty}\if@twocolumn\hbox{}\newpage\fi\fi}

% Redefinition de l'en-tete d'un chapitre.
\def\@makechapterhead#1{%
   \ifscreen
      \relax
   \else
      \vspace*{30\p@}%
   \fi
   {%
   \parindent \z@ \raggedleft \normalfont
   \ifnum \c@secnumdepth >\m@ne
      \vskip 20\p@
      \textcolor{ch@ptercolor}{\hrulefill}
      \raisebox{-13\p@}{\Huge\bfseries\fcolorbox{white}{ch@ptercolor}{\scalebox{4}{\textcolor{white}{\thechapter}}}} % \@chapapp\space \thechapter
      \par\nobreak
      \ifscreen
         \relax
      \else
         \vskip 20\p@
      \fi
   \fi
   \interlinepenalty\@M
   \raggedright
   \Huge \bfseries \textcolor{ch@ptercolor}{#1}\par\nobreak
   \ifscreen
      \vskip 30\p@
   \else
      \vskip 40\p@
   \fi}}

% Redefinition de l'en-tete d'un chapitre étoilé
\def\@makeschapterhead#1{%
   \ifscreen
      \relax
   \else
      \vspace*{30\p@}%
   \fi
   {%
   \parindent \z@ \raggedleft \normalfont
   \interlinepenalty\@M
   \raggedright
   \Huge\bfseries\textcolor{ch@ptercolor}{#1}\par\nobreak
   \ifscreen
      \vskip 30\p@
   \else
      \vskip 40\p@
   \fi}}

% Redefinition de l'en-tete d'une partie
\def\@part[#1]#2{%
   \ifnum \c@secnumdepth > -2\relax
      \refstepcounter{part}%
      \addcontentsline{toc}{part}{\thepart\hspace{1em}#1}%
   \else
      \addcontentsline{toc}{part}{#1}%
   \fi
   \markboth{}{}%
   {%
   \centering
   \interlinepenalty \@M
   \normalfont
   \ifnum \c@secnumdepth > -2\relax
      \hfill\Huge\bfseries\fcolorbox{white}{ch@ptercolor}{\scalebox{6}{\textcolor{white}{\thepart}}}\par\nobreak
      \vfill
   \fi
   \Huge\bfseries\color{ch@ptercolor}#2\par}\@endpart}
\def\@spart#1{%
   {%
   \centering
   \interlinepenalty \@M
   \normalfont
   \Hugecover\bfseries \color{ch@ptercolor}#1\par}\@endpart}
\def\@endpart{\vfill\vfill\vfill\newpage
               \if@twoside
               \if@openright
                  \null
                  \thispagestyle{empty}%
                  \newpage
               \fi
               \fi
               \if@tempswa
                  \twocolumn
               \fi}

% Environnement de remerciements.
\newenvironment{acknowledgements}{\section*{\acknowledgementsname}}{\par}

\bibliographystyle{alpha}

% Fiches de suivi.

\newcounter{sem@ine}

\setcounter{sem@ine}{1}

\newcounter{incrementb@sheures}
\newcounter{incrementb@sminutes}
\newcounter{incrementh@utheures}
\newcounter{incrementh@utminutes}

\setcounter{incrementb@sheures}{10}
\setcounter{incrementb@sminutes}{00}
\setcounter{incrementh@utheures}{12}
\setcounter{incrementh@utminutes}{30}

\newcounter{b@sheures}
\newcounter{b@sminutes}
\newcounter{h@utheures}
\newcounter{h@utminutes}

\setcounter{b@sheures}{\value{incrementb@sheures}}
\setcounter{b@sminutes}{\value{incrementb@sminutes}}
\setcounter{h@utheures}{\value{incrementh@utheures}}
\setcounter{h@utminutes}{\value{incrementh@utminutes}}

\newcounter{accumulateurheuresA}
\newcounter{accumulateurminutesA}
\newcounter{accumulateurheuresB}
\newcounter{accumulateurminutesB}

\setcounter{accumulateurheuresA}{0}
\setcounter{accumulateurminutesA}{0}
\setcounter{accumulateurheuresB}{0}
\setcounter{accumulateurminutesB}{0}

\newcounter{ratioAbas}
\newcounter{ratioBbas}
\newcounter{ratioAhaut}
\newcounter{ratioBhaut}

\newenvironment{fichesuivi}[2]
	{
	\noindent
   \begin{minipage}{\columnwidth}
	   \hrule
	   \bigskip
	   \begin{center}
	      \textbf{\WeeklyReport\hypertarget{sem:\arabic{sem@ine}}{\arabic{sem@ine}}}

	      \textbf{\WeeklyReportFrom\ #1 \WeeklyReportTo\ #2}
	   \end{center}
	   \hrule
	\end{minipage}

	\bigskip
	}
	{
	\vspace{5\baselineskip}

	\addtocounter{sem@ine}{1}
	\addtocounter{b@sheures}{\value{incrementb@sheures}}
	\addtocounter{b@sminutes}{\value{incrementb@sminutes}}
	\ifthenelse{\value{b@sminutes}>59}
	   { % then
	   \addtocounter{b@sheures}{1}
	   \addtocounter{b@sminutes}{-60}
	   }
	   {% else
	   }
	\addtocounter{h@utheures}{\value{incrementh@utheures}}
	\addtocounter{h@utminutes}{\value{incrementh@utminutes}}
	\ifthenelse{\value{h@utminutes}>59}
	   {% then
	   \addtocounter{h@utheures}{1}
	   \addtocounter{h@utminutes}{-60}
	   }
	   {% else
	   }
	}

\newcommand{\tempstravailA}[2]
   {%
	\ifnum #2<0
	   \ClassError{report-rd-info}
	              {Erreur sur le temps de travail : Les minute ne peuvent pas \^etre n\'egatives}
	              {}
	\fi
	\ifnum #2>59
	   \ClassError{report-rd-info}
	              {Erreur sur le temps de travail : Les minute ne peuvent d\'epasser 59}
	              {}
	\fi
   \ifx\@authorA\@empty
      \ClassError{report-rd-info}
                 {Erreur sur le premier bin\^ome : Il doit y avoir au moins un auteur}
                 {}
   \else
      \noindent \WeeklyAuthorWork\ \@authorA
   \fi
   : #1 h #2 m\par
	\addtocounter{accumulateurheuresA}{#1}
	\addtocounter{accumulateurminutesA}{#2}
	\ifthenelse{\value{accumulateurminutesA}>59}
	   {% then
	   \addtocounter{accumulateurheuresA}{1}
	   \addtocounter{accumulateurminutesA}{-60}
	   }
	   {% else
	   }
	\setcounter{ratioAbas}{100 * (\value{accumulateurheuresA} * 60 + \value{accumulateurminutesA}) /
	                             (\value{b@sheures}           * 60 + \value{b@sminutes})}
	\setcounter{ratioAhaut}{100 * (\value{accumulateurheuresA} * 60 + \value{accumulateurminutesA}) /
	                              (\value{h@utheures}          * 60 + \value{h@utminutes})}
	\immediate\write\summaryfile{      \string\hyperlink{sem:\arabic{sem@ine}}{\arabic{sem@ine}} &
	                                   \arabic{b@sheures} &
	                                   \ifnum\value{b@sminutes}<10 0\fi\arabic{b@sminutes} &
	                                   \arabic{h@utheures} &
	                                   \ifnum\value{h@utminutes}<10 0\fi\arabic{h@utminutes} &
	                                   #1 &
	                                   #2 &
	                                   \arabic{accumulateurheuresA} &
	                                   \ifnum\value{accumulateurminutesA}<10 0\fi\arabic{accumulateurminutesA} &
	                                   \arabic{ratioAbas} &
	                                   \string\textcolor{r@tiomaxcolor}{(\arabic{ratioAhaut})} & }%
   \ifx\@authorB\@empty
		\immediate\write\summaryfile{        &
		                                     &
		                                     &
		                                     &
		                                     &
	                                     \string\tabularnewline
	                                     \string\hline}%
	\fi
   }

\newcommand{\tempstravailB}[2]
   {%
   \ifx\@authorB\@empty
   	   \ClassError{report-rd-info}
		           {Erreur sur le second bin\^ome : Il n'y a qu'un seul auteur}
		           {}
   \else
		\ifnum #2<0
		   \ClassError{report-rd-info}
		              {Erreur sur le temps de travail : Les minute ne peuvent pas \^etre n\'egatives}
		              {}
		\fi
		\ifnum #2>59
		   \ClassError{report-rd-info}
		              {Erreur sur le temps de travail : Les minute ne peuvent d\'epasser 59}
		              {}
		\fi
      \noindent \WeeklyAuthorWork\ \@authorB : #1 h #2 m\par
		\addtocounter{accumulateurheuresB}{#1}
		\addtocounter{accumulateurminutesB}{#2}
		\ifthenelse{\value{accumulateurminutesB}>59}
		   {% then
		   \addtocounter{accumulateurheuresB}{1}
		   \addtocounter{accumulateurminutesB}{-60}
		   }
		   {% else
		   }
		\setcounter{ratioBbas}{100 * (\value{accumulateurheuresB} * 60 + \value{accumulateurminutesB}) /
		                             (\value{b@sheures}           * 60 + \value{b@sminutes})}
		\setcounter{ratioBhaut}{100 * (\value{accumulateurheuresB} * 60 + \value{accumulateurminutesB}) /
		                              (\value{h@utheures}          * 60 + \value{h@utminutes})}
		\immediate\write\summaryfile{      #1 &
		                                   #2 &
		                                   \arabic{accumulateurheuresB} &
		                                   \ifnum\value{accumulateurminutesB}<10 0\fi\arabic{accumulateurminutesB} &
		                                   \arabic{ratioBbas} &
		                                   \string\textcolor{r@tiomaxcolor}{(\arabic{ratioBhaut})}
	                                      \string\tabularnewline
	                                      \string\hline}%
   \fi
   }

\newenvironment{travaileffectue}
	{\noindent\textbf{\WeeklyWorkDone.}\par}
	{}

\newenvironment{travailnoneffectue}
	{\noindent\textbf{\WeeklyWorkNotDone.}\par}
	{}

\newenvironment{echange}
	{\noindent\textbf{\WeeklyExchange.}\par}
	{}

\newenvironment{planification}
	{\noindent\textbf{\WeeklyNextPlanning.}\par}
	{}

% Recapitulatif du travail hebdomadaire
\newwrite\summaryfile
\immediate\openout\summaryfile=report-rd-summary.tex

\newcommand{\printweeksummary}
   {%
	\immediate\closeout\summaryfile
	\makeatletter
	\begin{table*}
	   \centering
	   \begin{tabular}{|c||r@{ : }r|r@{ : }r||r@{ : }r|r@{ : }r|rr||r@{ : }r|r@{ : }r|rr|}
	      \cline{2-17}
	      \multicolumn{1}{c|}{} &
	      \multicolumn{4}{c||}{\ifenglish Planned\else Temps\fi} &
	      \multicolumn{6}{c||}{\@authorAfirstname} &
	      \multicolumn{6}
	                  {c|}
	                  {%
	                  \ifx\@authorB\@empty
	                     \relax
	                  \else
	                     \@authorBfirstname
	                  \fi
	                  }
	      \tabularnewline
	      \multicolumn{1}{c|}{} &
	      \multicolumn{4}{c||}{\ifenglish Time\else pr\'evu\fi} &
	      \multicolumn{6}{c||}{\textsc{\@authorAlastname}} &
	      \multicolumn{6}
	                  {c|}
	                  {%
	                  \ifx\@authorB\@empty
	                     \relax
	                  \else
	                     \textsc{\@authorBlastname}%
	                  \fi
	                  }
	      \tabularnewline
	      \cline{2-17}
	      \multicolumn{1}{c|}{} &
	      \multicolumn{2}{c|}{\ifenglish low\else bas\fi} &
	      \multicolumn{2}{c||}{\ifenglish high\else haut\fi} &
	      \multicolumn{2}{c|}{\ifenglish weekly\else hebdo.\fi} &
	      \multicolumn{2}{c|}{$\Sigma$} &
	      \multicolumn{2}{c||}{\multirow{2}{*}{\%}} &
	      \multicolumn{2}{c|}{\ifenglish weekly\else hebdo.\fi} &
	      \multicolumn{2}{c|}{$\Sigma$} &
	      \multicolumn{2}{c|}{\multirow{2}{*}{\%}}
	      \tabularnewline
	      \cline{1-9}\cline{12-15}
	      \ifenglish Week\else Semaine\fi & h & m & h & m & h & m & h & m & & & h & m & h & m & &
	                                   \tabularnewline
	      \hline
	                                   \hline

		   \input{report-rd-summary.tex}

	   \end{tabular}
		\ifenglish
		   \caption{Advance of the project with respect to the planned minimal theoretical time (respectively, a high involvement)}
		\else
		   \caption{Avancement du projet par rapport au temps de travail th\'eorique minimal (respectivement haut)}
	   \fi
	   \label{tab:AvancementProjet}
	\end{table*}
	\makeatother

	\ifenglish
		Table~\ref{tab:AvancementProjet} summarises the rate at which the projects advances. Let us recall that the ``planned minimal theoretical time'' equals the time indicated in the programme plus 20\% of additional time corresponding to personal work, i.e., outside the timetable. The high limit corresponds to 50\% of aditional personal work.
	\else
		Le tableau~\ref{tab:AvancementProjet} r\'ecapitule le taux d'avancement du projet. Rappelons que le temps de travail th\'eorique \emph{minimal} correspond au temps indiqu\'e sur la maquette p\'edagogique auquel on ajoute un strict minimum de 20~\% correspondant au travail personnel hors emploi du temps. La partie << haute >> de la fourchette correspond \`a 50~\% de temps suppl\'ementaire au titre du travail personnel.
   \fi
   }


\endinput


%%% Local Variables:
%%% mode: latex
%%% TeX-master: t
%%% End:
