#include "scenes/GUITools.hpp"

GUITools::GUITools(void) : _slowDialogue(true) { }

GUITools::~GUITools(void) { }

void GUITools::_displayDialogue(GUI *graphicUI, int *searchWord, int *lastWord,
								 int *startStrIdx, std::string name,
								 std::string imgName, std::string str,
								 bool isImgLeft, size_t maxCharPerLine,
								 int nbrOfLine, nk_flags textPosition,
								 std::string fontText, std::string fontTitle) {
	size_t maxCPL =
		((WINDOW_W / 4) * 3 - (((WINDOW_H / 4) - 45) - (WINDOW_W / 4)) - 40) /
		8.5;
	maxCharPerLine = maxCharPerLine > maxCPL ? maxCPL : maxCharPerLine;
 	int nbrOfLineTmp = (((WINDOW_H / 4) - 45) / 22) - 2;
	nbrOfLine = nbrOfLine > nbrOfLineTmp ? nbrOfLineTmp : nbrOfLine;
	if (_slowDialogue) {
		if (*searchWord < (int)str.size()) {
			*searchWord += 1;
			if (str[*searchWord] == ' ') *lastWord = *searchWord;
		} else
			*lastWord = *searchWord;
		if (*lastWord - *startStrIdx >= (int)maxCharPerLine * (nbrOfLine - 1))
			*startStrIdx += maxCharPerLine;
	}
	_slowDialogue = !_slowDialogue;
	std::string displayableStr =
		str.substr(*startStrIdx, *lastWord - *startStrIdx);
	graphicUI->uiDialogBox(name.c_str(), imgName, displayableStr.c_str(),
						   isImgLeft, maxCharPerLine, nbrOfLine, textPosition,
						   fontText, fontTitle);
}

bool GUITools::_displayPauseMenu(GUI *graphicUI, int *_newSceneIdx, int restartIdx, int leaveIdx) {
	bool res = true;
	if (graphicUI->uiStartBlock(
			"PauseMenu", "Pause",
			nk_rect((WINDOW_W / 2) - (WINDOW_W / 8), (WINDOW_H / 3),
					WINDOW_W / 4, WINDOW_H / 3),
			NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
								"Resume", "", "14_BOMBERMAN")) {
			res = false;
		}
		if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
								"Restart level", "", "14_BOMBERMAN")) {
			*_newSceneIdx = restartIdx;
		}
		if (graphicUI->uiButton(WINDOW_W / 4, (WINDOW_H / 9) - 8, 0,
								"Quit level", "", "14_BOMBERMAN")) {
			*_newSceneIdx = leaveIdx;
		}
		if (graphicUI->uiButton(0, 0, 0, "", "", "")) {	}
	}
	graphicUI->uiEndBlock();
	return res;
}
