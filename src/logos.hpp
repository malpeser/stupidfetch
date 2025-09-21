#pragma once
#include <vector>
#include <string>

/* These were taken from screenfetch (https://github.com/kittykatt/screenfetch) */

std::vector<std::string> redhat = { // Newlines are temporary, the color is red.
"            .MMM..:MMMMMMM                 \n",
"           MMMMMMMMMMMMMMMMMM              \n",
"           MMMMMMMMMMMMMMMMMMMM.           \n",
"          MMMMMMMMMMMMMMMMMMMMMM           \n",
"         ,MMMMMMMMMMMMMMMMMMMMMM:          \n",
"         MMMMMMMMMMMMMMMMMMMMMMMM          \n",
"   .MMMM'  MMMMMMMMMMMMMMMMMMMMMM          \n",
"  MMMMMM    `MMMMMMMMMMMMMMMMMMMM.         \n",
" MMMMMMMM      MMMMMMMMMMMMMMMMMM .        \n",
" MMMMMMMMM.       `MMMMMMMMMMMMM' MM.      \n",
" MMMMMMMMMMM.                     MMMM     \n",
" `MMMMMMMMMMMMM.                 ,MMMMM.   \n",
"  `MMMMMMMMMMMMMMMMM.          ,MMMMMMMM.  \n",
"     MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM  \n",
"       MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM:  \n",
"          MMMMMMMMMMMMMMMMMMMMMMMMMMMMMM   \n",
"             `MMMMMMMMMMMMMMMMMMMMMMMM:    \n",
"                 ``MMMMMMMMMMMMMMMMM'      \n"
};

std::vector<std::string> manjaro = { // Newlines are temporary, the color is green.
" ██████████████████  ████████    \n",
" ██████████████████  ████████    \n",
" ██████████████████  ████████    \n",
" ██████████████████  ████████    \n",
" ████████            ████████    \n",
" ████████  ████████  ████████    \n",
" ████████  ████████  ████████    \n",
" ████████  ████████  ████████    \n",
" ████████  ████████  ████████    \n",
" ████████  ████████  ████████    \n",
" ████████  ████████  ████████    \n",
" ████████  ████████  ████████    \n",
" ████████  ████████  ████████    \n",
" ████████  ████████  ████████    \n",
"                                 \n"
};

std::vector<std::string> neon = {
"              `..---+/---..`               \n",
"          `---.``   ``   `.---.`           \n",
"       .--.`        ``        `-:-.        \n",
"     `:/:     `.----//----.`     :/-       \n",
"    .:.    `---`          `--.`    .:`     \n",
"   .:`   `--`                .:-    `:.    \n",
"  `/    `:.      `.-::-.`      -:`   `/`   \n",
"  /.    /.     `:++++++++:`     .:    .:   \n",
" `/    .:     `+++++++++++/      /`   `+`  \n",
" /+`   --     .++++++++++++`     :.   .+:  \n",
" `/    .:     `+++++++++++/      /`   `+`  \n",
"  /`    /.     `:++++++++:`     .:    .:   \n",
"  ./    `:.      `.:::-.`      -:`   `/`   \n",
"   .:`   `--`                .:-    `:.    \n",
"    .:.    `---`          `--.`    .:`     \n",
"     `:/:     `.----//----.`     :/-       \n",
"       .-:.`        ``        `-:-.        \n",
"          `---.``   ``   `.---.`           \n",
"              `..---+/---..`               \n"
};

std::vector<std::string> arch = {
"                   -`                 \n",
"                  .o+`                \n",
"                 `ooo/                \n",
"                `+oooo:               \n",
"               `+oooooo:              \n",
"               -+oooooo+:             \n",
"             `/:-:++oooo+:            \n",
"            `/++++/+++++++:           \n",
"           `/++++++++++++++:          \n",
"          `/+++ooooooooooooo/`        \n",
"         ./ooosssso++osssssso+`       \n",
"        .oossssso-````/ossssss+`      \n",
"       -osssssso.      :ssssssso.     \n",
"      :osssssss/        osssso+++.    \n",
"     /ossssssss/        +ssssooo/-    \n",
"   `/ossssso+/:-        -:/+osssso+-  \n",
"  `+sso+:-`                 `.-/+oso: \n",
" `++:.                           `-/+/\n",
" .`                                 `/\n",
};

// WARNING: THIS ONE HAS THREE COLORS. OH crap
/*
c1 = red (\033[38;5;196m)
c2 = lite red (\033[38;5;166m)
c3 = bold yellow (\033[1;33m)
*/
std::vector<std::string> ubuntu = {
"\033[38;5;166m                          ./+o+-      \n",
"\033[38;5;196m                  yyyyy- \033[38;5;166m-yyyyyy+     \n",
"\033[38;5;196m               \033[38;5;196m://+//////\033[38;5;166m-yyyyyyo     \n",
"\033[1;33m           .++ \033[38;5;196m.:/++++++/-\033[38;5;166m.+sss/`     \n",
"\033[1;33m         .:++o:  \033[38;5;196m/++++++++/:--:/-     \n",
"\033[1;33m        o:+o+:++.\033[38;5;196m`..```.-/oo+++++/    \n",
"\033[1;33m       .:+o:+o/.\033[38;5;196m          `+sssoo+/   \n",
"\033[38;5;196m  .++/+:\033[1;33m+oo+o:`\033[38;5;196m             /sssooo.  \n",
"\033[38;5;196m /+++//+:\033[1;33m`oo+o\033[38;5;196m               /::--:.  \n",
"\033[38;5;196m +/+o+++\033[1;33m`o++o\033[38;5;166m               ++////.  \n",
"\033[38;5;196m  .++.o+\033[1;33m++oo+:`\033[38;5;166m             /dddhhh.  \n",
"\033[1;33m       .+.o+oo:.\033[38;5;166m          `oddhhhh+   \n",
"\033[1;33m        +.++o+o`\033[38;5;166m`-````.:ohdhhhhh+    \n",
"\033[1;33m         `:o+++ \033[38;5;166m`ohhhhhhhhyo++os:     \n",
"\033[1;33m           .o:\033[38;5;166m`.syhhhhhhh/\033[1;33m.oo++o`     \n",
"\033[38;5;166m               /osyyyyyyo\033[1;33m++ooo+++/    \n",
"\033[38;5;166m                   ````` \033[1;33m+oo+++o:    \n",
"\033[1;33m                          `oo++.      \n"
};

// c1 = white c2 = light red
std::vector<std::string> debian = {
"\033[1;37m         _,met$$$$$gg.          \n",
"\033[1;37m      ,g$$$$$$$$$$$$$$$P.       \n",
"\033[1;37m    ,g$$P\"\"       \"\"\"Y$$.\".     \n",
"\033[1;37m   ,$$P'              `$$$.     \n",
"\033[1;37m  ',$$P       ,ggs.     `$$b:   \n",
"\033[1;37m  `d$$'     ,$P\"\'   \033[38;5;166m.\033[1;37m    $$$    \n",
"\033[1;37m   $$P      d$\'     \033[38;5;166m,\033[1;37m    $$P    \n",
"\033[1;37m   $$:      $$.   \033[38;5;166m-\033[1;37m    ,d$$'    \n",
"\033[1;37m   $$;      Y$b._   _,d$P'     \n",
"\033[1;37m   Y$$.    \033[38;5;166m`.\033[1;37m`\"Y$$$$P\"'         \n",
"\033[1;37m   `$$b      \033[38;5;166m\"-.__              \n",
"\033[1;37m    `Y$$                        \n",
"\033[1;37m     `Y$$.                      \n",
"\033[1;37m       `$$b.                    \n",
"\033[1;37m         `Y$$b.                 \n",
"\033[1;37m            `\"Y$b._             \n",
"\033[1;37m                `\"\"\"\"           \n",
"\033[1;37m                                \n"
};
