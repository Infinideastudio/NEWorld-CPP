//
// Copyright 2333 ****inglogging
//

#ifndef ABCDEFGHIJKLMNOPQRSTUVWXYZ
#define ABCDEFGHIJKLMNOPQRSTUVWXYZ

#include "LogSystem.hpp"

#include <ctime>
#include <cstdlib>

#ifndef NO_FUCK_ABILITY
constexpr unsigned xxxx = 11;
constexpr char const *saying[] = {
    "have you ever mooed today?\n"
    "(me) MOO~~~",

    "Have you ever mooed yesterday?\n"
    "(yesterday) Yes!",

    "Have you ever mooed tomorrow?\n"
    "(qiaozhanrong) 喵~~~",

    "Who makes so many bugs?\n"
    "Peter.",

    "RMS means RMS means Stallman.",

    "(A) Are you debugging?\n"
    "(B) No, I'm watching you.",

    "again: goto again;",

    "(News) GCC supported std::regex!",

    "Linux is the best operated system!",

    "$ make love\n"
    "(error) No target to make 'love'.",

    "$ [whereis my head?\n"
    "sh: Missing ']'"
};
#endif

namespace LogFucker {
void Fuck();
}  // namespace LogFucker

void LogFucker::Fuck() {
#ifndef NO_FUCK_ABILITY
    time_t t =
        time
        (
            nullptr
        )
        ;
    srand
    (
        static_cast
        <
        unsigned
        >
        (
            localtime
            (
                &t
            )
            ->tm_sec
        )
    )
    ;
    unsigned index =
        static_cast
        <
        unsigned
        >
        (
            static_cast
            <
            unsigned
            >
            (
                rand
                (
                )
            )
            %
            xxxx
        )
        ;
    ::LogSystem::Debug(saying[index]);
#endif
}

#endif  // ABCDEFGHIJKLMNOPQRSTUVWXYZ
