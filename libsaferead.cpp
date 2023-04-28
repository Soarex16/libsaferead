//
// Created by Soarex16.
//

#include "libsaferead.h"
#include "csignal"
#include <stdio.h>
#include <setjmp.h>

sigjmp_buf point;

void handler(int signum, siginfo_t * siginfo, void *code) {
    if (signum == SIGSEGV || signum == SIGBUS) {
        siglongjmp(point, 1);
    }
}

std::optional<std::uint8_t> safe_read_uint8(const std::uint8_t *p) {
    struct sigaction act, old_sig_segv_act, old_sig_bus_act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = handler;

    if (sigaction(SIGSEGV, &act, &old_sig_segv_act) == -1) {
        perror("could not initialize SIGSEGV signal handler");
        return std::nullopt;
    }

    if (sigaction(SIGBUS, &act, &old_sig_bus_act) == -1) {
        perror("could not initialize SIGBUS signal handler");
        return std::nullopt;
    }

    std::optional<std::uint8_t> result;
    if(sigsetjmp(point, 42) == 0) {
        std::uint8_t value = *p;
        result = { value };
    } else {
        result = std::nullopt;
    }

    // cleanup
    sigaction(SIGSEGV, &old_sig_segv_act, nullptr);
    sigaction(SIGSEGV, &old_sig_bus_act, nullptr);

    return result;
}
