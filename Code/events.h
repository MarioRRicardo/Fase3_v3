// ###########################################
// ####### ESTRUTURAS DE DADOS


// ###########################################
// ####### FUNÇÕES
void keyboardEvents(unsigned char key, int x, int y);
void keyboardSpecialEvents(int key, int x, int y);
void mouseEvents(int button, int state, int x, int y);
void mouseMove(int x, int y);
//void wheelEvents(int button, int dir, int x, int y);
void menuEvents(int option);


void processMouseButtons(int button, int state, int xx, int yy);
void processMouseMotion(int xx, int yy);