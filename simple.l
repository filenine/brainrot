%{
#include "lang.tab.h"
%}

%%

"bruh" { return BREAK; }
"+"    { return PLUS; }
[0-9]+ { return NUMBER; }

%%
