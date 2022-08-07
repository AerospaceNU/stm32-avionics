/*
 * task_cli_version.cpp
 *
 *  Created on: Jul 18, 2022
 *      Author: sam
 */

#include <cstdio>

#include "cli.h"
#include "cli_tasks.h"
#include "git_info.h"

/**
 * @brief Sends Git version and tag info over CLI
 */
void cli_tasks::cliVersion() {
  char cliStr[70];
  // Send ack of command
  cliSendAck(true, nullptr);
  snprintf(cliStr, sizeof(cliStr), "Git Commit/Tag Info:\t %s\r\n",
           GIT_DESCRIBE);
  cliSend(cliStr);
  snprintf(cliStr, sizeof(cliStr), "Last Commit On:\t\t %s\r\n",
           GIT_COMMIT_TIME);
  cliSend(cliStr);
  snprintf(cliStr, sizeof(cliStr), "With Message:\t\t %s\r\n",
           GIT_COMMIT_MESSAGE);
  cliSend(cliStr);

  cliSendComplete(true, nullptr);
}
