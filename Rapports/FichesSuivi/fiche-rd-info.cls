%=============================================================================
% Classe fiche-rd-info (basée sur la classe standard ``report'').
% Rapport de travail de recherche et développement du département d'informatique de Polytech'Nantes
% Auteur : José Martinez (originellement basé sur le rapport de recherche du LINA de Frédéric Goualard)
%=============================================================================
\def\REPORT@RD@INFO@Version{2.0}
\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{fiche-rd-info}[2010/09/03 fiche-rd-info version \REPORT@RD@INFO@Version]
\typeout{(c) 2008-2010 École polytechnique de l'université de Nantes}
\typeout{ }

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Gestion des options =
% Déclaration des options possibles
\newif\if@inEnglish
\DeclareOption{english}{\@inEnglishtrue}
\DeclareOption{francais}{\@inEnglishfalse}

\DeclareOption{10pt}{\gdef\@pt@size{10pt}}
\DeclareOption{11pt}{\gdef\@pt@size{11pt}}
\DeclareOption{12pt}{\gdef\@pt@size{12pt}}

\newif\if@inScreenMode
\@inScreenModefalse
\DeclareOption{screen}{\@inScreenModetrue}

\ExecuteOptions{francais,final,12pt}
\ProcessOptions\relax

%<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Fin gestion des options =

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Chargement de paquetages additionnels =

\PassOptionsToClass{\@pt@size,final}{article}
\LoadClass{report}

% Attention : ne pas déplacer l'appel pour charger 'color'
\RequirePackage{times}
\RequirePackage{a4wide}
\RequirePackage[english,francais]{babel}
\if@inEnglish
\AtBeginDocument{%
  \selectlanguage{english}
}
\else
\AtBeginDocument{%
  \selectlanguage{francais}
}
\fi

%<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< fin chargement de paquetages additionnels =


\newlength{\titlewidth}
\setlength{\titlewidth}{16cm}

%>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Localisation
\if@inEnglish
  \AtBeginDocument{%
		\def\today{\ifcase\month\or
		January\or February\or March\or April\or May\or June\or
		July\or August\or September\or October\or November\or December\fi
		~\number\year}
				
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
		
		\newcommand{\WeeklyReport}{Weekly Report}
		\newcommand{\WeeklyReportFrom}{From}
		\newcommand{\WeeklyReportTo}{to}
		\newcommand{\WeeklyAuthorWork}{Working time of}
		\newcommand{\WeeklyWorkDone}{Work done}
		\newcommand{\WeeklyWorkNotDone}{Work not yet done}
		\newcommand{\WeeklyExchange}{Communication with the client}
		\newcommand{\WeeklyNextPlanning}{Next week planning}
  }
\else
   \AtBeginDocument{%
		\def\today{\ifcase\month\or
		 janvier\or f\'evrier\or mars\or avril\or mai\or juin\or
		 juillet\or août\or septembre\or octobre\or novembre\or d\'ecembre\fi
		 ~\number\year}
				
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
		
		\newcommand{\WeeklyReport}{Fiche de suivi de la semaine 15}
		\newcommand{\WeeklyReportFrom}{du}
		\newcommand{\WeeklyReportTo}{au}
		\newcommand{\WeeklyAuthorWork}{Temps de travail de}
		\newcommand{\WeeklyWorkDone}{Travail effectu\'e}
		\newcommand{\WeeklyWorkNotDone}{Travail non effectu\'e}
		\newcommand{\WeeklyExchange}{\'Echanges avec le commanditaire}
		\newcommand{\WeeklyNextPlanning}{Planification pour la semaine prochaine}
	}
\fi
%<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Fin customisations suivant la langue =

\renewcommand{\author}
   {
   \ClassError{fiche-rd-info}
              {Pas d'auteur : Indiquez au moins un auteur avec la macro \string\authorA}
              {}
   }
\def\@authorA{\ClassError{fiche-rd-info}
                         {Pas d'auteur : Indiquez au moins un auteur avec la macro \string\authorA}
                         {}}
\newcommand{\authorA}[2]{\gdef\@authorAfirstname{#1}\gdef\@authorAlastname{#2}}

\def\@authorBfirstname{+}
\def\@authorBlastname{+}
\newcommand{\authorB}[2]{\gdef\@authorBfirstname{#1}\gdef\@authorBlastname{#2}}


\newenvironment{fichesuivi}[2]
	{
	\noindent
   \begin{minipage}{\columnwidth}
	   \hrule
	   \bigskip
	   \begin{center}
	      \textbf{\WeeklyReport}
	      
	      \textbf{\WeeklyReportFrom\ #1 \WeeklyReportTo\ #2}
	   \end{center}
	   \hrule
	\end{minipage}
	
	\bigskip
	}
	{
	\vspace{5\baselineskip}
	}
	
\newcommand{\tempstravailA}[2]
   {%
	\ifnum #2<0
	   \ClassError{fiche-rd-info}
	              {Erreur sur le temps de travail : Les minute ne peuvent pas \^etre n\'egatives}
	              {}
	\fi
	\ifnum #2>59
	   \ClassError{fiche-rd-info}
	              {Erreur sur le temps de travail : Les minute ne peuvent d\'epasser 59}
	              {}
	\fi
   \if\@authorAfirstname+%
   	\ClassError{fiche-rd-info}
		           {Erreur sur le premier bin\^ome : Il doit y avoir au moins un auteur}
		           {}
   \else
      \noindent \WeeklyAuthorWork\ \@authorAfirstname\ \textsc{\@authorAlastname}
   \fi
   : #1 h #2 m\par
   }
   
\newcommand{\tempstravailB}[2]
   {%
   \if\@authorBfirstname+%
   	\ClassError{fiche-rd-info}
		           {Erreur sur le second bin\^ome : Il n'y a qu'un seul auteur}
		           {}
   \else
		\ifnum #2<0
		   \ClassError{fiche-rd-info}
		              {Erreur sur le temps de travail : Les minute ne peuvent pas \^etre n\'egatives}
		              {}
		\fi
		\ifnum #2>59
		   \ClassError{fiche-rd-info}
		              {Erreur sur le temps de travail : Les minute ne peuvent d\'epasser 59}
		              {}
		\fi
      \noindent \WeeklyAuthorWork\ \@authorBfirstname\ \textsc{\@authorBlastname} : #1 h #2 m\par
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


\endinput


%%% Local Variables:
%%% mode: latex
%%% TeX-master: t
%%% End:
