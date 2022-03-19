
#include "state_cli_version.h"

#include <stdio.h>

#include "cli.h"

#if __has_include("git_info.h")
#include "git_info.h"
#else
#define GIT_INFO "Build version info not present"
#endif

void CliVersionState::init() {
	// Send ack of command
	cliSendAck(true, nullptr);
}

EndCondition_t CliVersionState::run() {
	// Send git version info
	char cliStr[80];

	sprintf(cliStr, "Build Version Info:\r\n");
	cliSend(cliStr);
	sprintf(cliStr, "%s", GIT_INFO);
	cliSend(cliStr);

	return EndCondition_t::CliCommandComplete;
}

void CliVersionState::cleanup() {
	cliSendComplete(true, nullptr);
}
