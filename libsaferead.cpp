//
// Created by Soarex16.
//

#include "libsaferead.h"
#include "csignal"

volatile sig_atomic_t segv_occurred = false;

void handler(int signum, siginfo_t * siginfo, void *code) {
    if (signum == SIGSEGV || signum == SIGBUS) {
        segv_occurred = true;
    }
}

std::optional<std::uint8_t> safe_read_uint8(const std::uint8_t *p) {
    struct sigaction act, old_sig_segv_act, old_sig_bus_act;
    act.sa_flags = SA_SIGINFO | SA_RESTART;
    act.sa_sigaction = handler;

    if (sigaction(SIGSEGV, &act, &old_sig_segv_act) == -1) {
        perror("could not initialize SIGSEGV signal handler");
        return std::nullopt;
    }

    if (sigaction(SIGBUS, &act, &old_sig_bus_act) == -1) {
        perror("could not initialize SIGBUS signal handler");
        return std::nullopt;
    }

    // actual logic
    auto res = *p;
    //    raise(SIGSEGV); // only for test

    // cleanup
    sigaction(SIGSEGV, &old_sig_segv_act, nullptr);
    sigaction(SIGSEGV, &old_sig_bus_act, nullptr);

    if (segv_occurred) {
        segv_occurred = false;
        return std::nullopt;
    }

    return { res };
}
