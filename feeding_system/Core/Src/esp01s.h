#ifndef ESP01S_H
#define ESP01S_H

void Esp01s_Init(char* ip, char* password, char* port);
void wait_for_esp(char* key);
void espSend(char* message);
void addNewlineIfNeeded(char* message);
#endif /* ESP01S_H */