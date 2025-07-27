#ifndef __TEXT_H__
#define __TEXT_H__

#include "MenuStrings.h"
#include "Translator.h"

class Text;
class InputStream;

// -------------------
// Localization Class
// -------------------

class Localization
{
private:

public:
	static constexpr int MAXBUFFERS = 7;
	static constexpr int MAX_STRING_ARGS = 50;
	static constexpr uint8_t HYPHEN = '-';

	static constexpr char* STRINGS_MENUS[4][49] = {
		{ // INGLES
		"Options", "Language", "Sound", "Video", "Input",
		"Sound", "SFX Volume:", "Music Volume:",
		"Video", "Window Mode:", "Windowed", "Borderless", "FullScreen", "VSync:", "Resolution:", "TinyGL:", "Apply Changes",
		"Input", "Touch Controls", "Bindings", "Controller",
		"Touch Controls", "Button Transparency:",
		"Bindings", "Unbound",
		"MOVEMENT",
		"Move Forward", "Move Backward", "Move Left", "Move Right", "Turn Left", "Turn Right",
		"OTHER",
		"Atk/Talk/Use", "Next Weapon", "Prev Weapon", "Pass Turn", "Automap", "Menu", "Items/Info", "Syringes", "Journal", "Reset Binds", " ",
		"Controller", "Vibration", "Vibration Intensity", "Deadzone", "Control layout:"
		},
		{ // FRANCES Appuyez sur une nouvelle touche pour
		"Options", "Langue", "Son", "Vid�o", "Commandes",
		"Son", "Volume effets :", "Volume musique :",
		"Vid�o", "Mod.Affichage :", "Fen�tr�", "Sans Bords", "Plein �cran", "VSync :", "R�solution :", "TinyGL :", "Valider Les Modifications",
		"Commandes", "Contr�les Tactiles", "Assignation Commandes", "Manette",
		"Contr�les Tactiles", "Transparence touches",
		"Assignation Commandes", "Non Assgn�e",
		"MOUVEMENT",
		"Avant", "Arri�re", "D�pl.Gauche", "D�pl.Droit", "Tourner Gauche", "Tourner Droit",
		"AUTRE",
		"Att/Parl/Util", "Arme Suivante", "Arme Pr�c�dente", "Pass.Tour", "Carte", "Menu", "Articles/Info", "Seringues", "Journal", "R�initialiser Les Commandes", " ",
		"Manette", "Vibration", "Intensit� de Vibration", "Zone Morte", "Config. Contr�les"
		},
		{ // ITALIANO Premi un nuovo tasto per
		"Opzioni", "Lingua", "Audio", "Video", "Comandi",
		"Audio", "Volume Effettii:", "Volume Musica:",
		"Video", "Mod.Finestra:", "Finestra", "Senza Bordi", "Sch. Intero", "VSync:", "Risoluzione:", "TinyGL:", "Applica Modifiche",
		"Comandi", "Controlli Touch", "Assegnazione Comandi", "Controller",
		"Controlli Touch", "Trasparenza Tasti",
		"Assegnazione Comandi", "Non Assegnato",
		"MOVIMENTO",
		"Avanti", "Indietro", "Sposta Sinistra", "Sposta Destra", "Ruota Sinistra", "Ruota Destra",
		"ALTRO",
		"Att/Parl/Usa", "Arma Successiva", "Arma Precedente", "Pass.Turno", "Automappa", "Menu", "Oggetti/Info", "Siringhe", "Diario", "Ripristina Assegnazioni", " ",
		"Controller", "Vibrazione", "Intensit� Vibrazione", "Zona Morta", "Schema controlli"
		},
		{ // ESPA�OL Presiona una nueva tecla para
		"Opciones", "Idioma", "Sonido", "V�deo", "Entrada",
		"Sonido", "Vol.Efectos:", "Vol.M�sica:",
		"V�deo", "Modo Ventana:", "En Ventana", "Sin Bordes", "Pant.Completa", "VSync:", "Resoluci�n:", "TinyGL:", "Aplicar Cambios",
		"Entrada", "Controles T�ctiles", "Asignaciones", "Mando",
		"Controles T�ctiles", "Transparencia de Botones:",
		"Asignaciones", "Sin vincular",
		"MOVIMIENTO",
		"Adelante", "Atras", "Mover Izquierda", "Mover Derecha", "Girar Izquierda", "Girar Derecha",
		"OTROS",
		"Atq/Habl/Usar", "Arma Siguiente", "Arma Anterior", "Pasar Turno", "Automapa", "Men�", "Art�culos/Info", "Jeringas", "Diario", "Restablecer Asignaciones", " ",
		"Mando", "Vibraci�n", "Intensidad de Vibraci�n", "Zona Muerta", "Dise�o de Control:"
		}
	};

    bool enableSDLTTF;
    bool enableMachineTextTranslation;
    Text* scratchBuffers[Localization::MAXBUFFERS];
	int bufferFlags;
	Text* dynamicArgs;
	int16_t argIndex[Localization::MAX_STRING_ARGS];
	int numTextArgs;
	bool selectLanguage;
	int defaultLanguage;
	int textSizes[Strings::FILE_MAX];
	int textCount[Strings::FILE_MAX];
	char** text;
	uint16_t** textMap;
	int* textIndex;
	int textLastType;
	int textCurChunk;
	int textCurOffset;
	InputStream* textChunkStream;

	// Constructor
	Localization();
	// Destructor
	~Localization();

	bool startup();
	static bool isSpace(char c);
	static bool isDigit(char c);
	static char toLower(char c);
	static char toUpper(char c);
	Text* getSmallBuffer();
	Text* getFatalErrorBuffer();
	Text* getLargeBuffer();
	void freeAllBuffers();
	void allocateText(int index);
	void unloadText(int index);
	void setLanguage(int language);
	void beginTextLoading();
	void finishTextLoading();
	void loadTextFromIndex(int i, int textLastType);
	void loadText(int index);
	void resetTextArgs();
	void addTextArg(char c);
	void addTextArg(int i);
	void addTextArg(Text* text, int i, int i2);
	void addTextArg(Text* text);
	void addTextIDArg(int16_t i);
	void addTextArg(int16_t i, int16_t i2);
	static constexpr int STRINGID(int16_t i, int16_t i2) {
		return i << 10 | i2;
	};
	void composeText(int i, Text* text);
	void composeText(int16_t n, int16_t n2, Text* text);
	void composeTextField(int i, Text* text);
	bool isEmptyString(int16_t i, int16_t i2);
	bool isEmptyString(int i);
	void getCharIndices(char c, int* i, int* i2);
};

// -----------
// Text Class
// -----------

class Text
{
private:
    bool containsValidChars();
public:
    bool isTranslated;
    wchar_t *chars;
	int _length;
	int stringWidth;

	// Constructor
	Text(int countChars);
	// Destructor
	~Text();

	bool startup();
	int length();
    void translateText();
    void setLength(int i);
	Text* deleteAt(int i, int i2);
    wchar_t charAt(int i);
	void setCharAt(char c, int i);
	Text* append(char c);
	Text* append(uint8_t c);
	Text* append(char* c);
	Text* append(int i);
	Text* append(Text* t);
	Text* append(Text* t, int i);
	Text* append(Text* t, int i, int i2);
	Text* insert(char c, int i);
	Text* insert(uint8_t c, int i);
	Text* insert(int i, int i2);
	Text* insert(char* c, int i);
	Text* insert(char* c, int i, int i2, int i3);
	int findFirstOf(char c);
	int findFirstOf(char c, int i);
	int findLastOf(char c);
	int findLastOf(char c, int n);
	int findAnyFirstOf(char* c, int i);
	void substring(Text* t, int i);
	void substring(Text* t, int i, int i2);
	void dehyphenate();
	void dehyphenate(int i, int i2);
	void trim();
	void trim(bool b, bool b2);
	int wrapText(int i);
	int wrapText(int i, char c);
	int wrapText(int i, int i2, char c);
	int wrapText(int i, int i2, int i3, char c);
	int insertLineBreak(int i, int i2, char c);
	int getStringWidth();
	int getStringWidth(bool b);
	int getStringWidth(int i, int i2, bool b);
	int getNumLines();
	bool compareTo(Text* t);
	bool compareTo(char* str);
	void toLower();
	void toUpper();
	void dispose();
};

#endif