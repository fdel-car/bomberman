#pragma once

#include "header.hpp"
#include "GUI/GUI.hpp"

class GUITools {

	public:
		GUITools(void);
		virtual ~GUITools(void);

	protected:

		void _displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
							  int *startStrIdx, std::string name,
							  std::string imgName, std::string text, bool isImgLeft,
							  size_t maxCharPerLine, int nbrOfLine,
							  nk_flags textPosition, std::string fontText,
							  std::string fontTitle);
		bool _displayPauseMenu(GUI *graphicUI, int *_newSceneIdx, int restartIdx, int leaveIdx);

		bool _slowDialogue;
};
