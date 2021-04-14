#pragma once
int BrowseForOpen();
char* ReadPixels(int f, int* NumCh);
char* Unwrap(char* Pbuff, int NumCh);
void Translate(char* titkos_uzenet, int NumCh);
void Post(char* NeptunId, char* message, int NumCh);