#include <errno.h>
#include <iostream>
#include <pwd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <ucontext.h>
#include <unistd.h>

void handler(int sig, siginfo_t *info, void *context) {

  pid_t sender_pid = info->si_pid;

  std::cout << "Recieved signal: " << sender_pid;

  uid_t sender_uid = info->si_uid;
  struct passwd *username = getpwuid(sender_uid);
  std::cout << "executed by :" << sender_uid << " (" << username->pw_name << ")"
            << std::endl;

  ucontext_t *ucontext = static_cast<ucontext_t *>(context);
  std::cout << "State of the context: ";
  std::cout << "EIP = " << ucontext->uc_mcontext.gregs[REG_RIP] << std::endl;
  std::cout << "EAX = " << ucontext->uc_mcontext.gregs[REG_RAX] << std::endl;
  std::cout << "EBX = " << ucontext->uc_mcontext.gregs[REG_RBX] << std::endl;
}

int main() {

  struct sigaction action;
  memset(&action, 0, sizeof(action));
  action.sa_flags = SA_SIGINFO;

  action.sa_sigaction = handler;

  if (sigaction(SIGUSR1, &action, NULL) < 0) {
    std::cerr << strerror(errno) << std::endl;
    exit(errno);
  }

  while (true) {
    sleep(10);
  }

  return 0;
}