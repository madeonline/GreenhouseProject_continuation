#pragma once
//--------------------------------------------------------------------------------------------------------------------------------------
#include "AbstractModule.h"
//--------------------------------------------------------------------------------------------------------------------------------------
class MCPModule : public AbstractModule
{
  private:
  public:
    MCPModule();

    bool ExecCommand(const Command& command, bool wantAnswer);
    void Setup();
    void Update();

};
//--------------------------------------------------------------------------------------------------------------------------------------
