#ifndef ESP01S_H
#define ESP01S_H
#include "data_structure.h"
void Esp01s_Init(char* ip, char* password, char* port);
void sendPlan(PlanList* planList);
void espSend(char* message,int enterFlag);
void addNewlineIfNeeded(char* message);
void waitCommand(char *command, uint32_t timeout_ms);
#endif /* ESP01S_H */