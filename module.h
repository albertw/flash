#if !defined (_MODULE_H)
#define _MODULE_H

void InitBackground(int argc, char **argv, FILE *fp, int *lc);
void InitBarClock(int argc, char **argv, FILE *fp, int *lc);
void InitMailCheck(int argc, char **argv, FILE *fp, int *lc);
void InitCountdown2000(int argc, char **argv, FILE *fp, int *lc);

void module(int wordc, char **wordv, FILE *fp, int *line_no);
void RunModuleFunction(int argc, char **argv);

#endif
