#include "..\Includes\Platform.h"

bool Platform::g_closing = false;
bool Platform::g_pausing = false;
bool Platform::g_resuming = false;

Platform::Platform ( unsigned int priority ) : ITask ( priority )
{}

Platform::~Platform ()
{}
