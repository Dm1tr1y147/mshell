#include "../include/shell.h"
#include "../include/utils.h"

/**
 * @brief Get IP adress of system
 * 
 * @return char* 
 */
char *get_ip_addr()
{
  struct ifaddrs *host, *tmp;
  getifaddrs(&host);
  tmp = host;
  char *ip = NULL;

  while (tmp)
  {
    if (tmp->ifa_addr && tmp->ifa_addr->sa_family == AF_INET)
    {
      struct sockaddr_in *pAddr = (struct sockaddr_in *)tmp->ifa_addr;
      ip = inet_ntoa(pAddr->sin_addr);
      if (strncmp(ip, "127", 3) != 0)
        break;
    }

    tmp = tmp->ifa_next;
  }

  freeifaddrs(host);

  return ip;
}

/**
 * @brief Creates prompt string
 * 
 * @return char* 
 */
char *compose_prompt()
{
  // New line
  char *prompt = strdup("");

  // Username
  char *username = strdup(getlogin());
  if (username == NULL)
    username = "none";
  prompt = realloc(prompt, strlen(prompt) + strlen("\033[97;44m") + strlen(username) + 2);
  prompt = strcat(prompt, "\033[97;44m");
  prompt = strcat(prompt, username);

  // Current host ip
  char *ip = get_ip_addr();
  if (ip == NULL)
    ip = "none";
  prompt = realloc(prompt, strlen(prompt) + 1 + strlen(ip) + strlen("\033[39m") + strlen("\033[0m") + 2);
  prompt = strcat(prompt, "@");
  prompt = strcat(prompt, ip);
  prompt = strcat(prompt, "\033[39m");
  prompt = strcat(prompt, "\033[0m");
  prompt = strcat(prompt, ":");

  // Current path
  char *full_path = get_curr_dir_name();
  prompt = realloc(prompt, strlen(prompt) + strlen("\033[92;1m") + strlen("\033[0m") + strlen(full_path) + 2);
  prompt = strcat(prompt, "\033[92;1m");
  prompt = strcat(prompt, full_path);
  prompt = strcat(prompt, "\033[0m");
  free(full_path);

  // Previous status
  if (term.last_status != 0)
  {
    char *status = malloc(snprintf(NULL, 0, " \033[91m%d\033[39m", term.last_status));
    sprintf(status, " \033[91m%d\033[39m", term.last_status);
    prompt = realloc(prompt, strlen(prompt) + strlen(status) + 1);
    prompt = strcat(prompt, status);
    free(status);
  }

  // Permissions specific character before user input
  prompt = realloc(prompt, strlen(prompt) + 4);
  if (getuid() == 0)
  {
    prompt = strcat(prompt, "\n# ");
  }
  else
    prompt = strcat(prompt, "\n% ");

  return prompt;
}