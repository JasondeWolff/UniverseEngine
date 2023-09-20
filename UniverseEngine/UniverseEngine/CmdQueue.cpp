#include "CmdQueue.h"

#include "CmdList.h"
#include "Semaphore.h"
#include "Fence.h"

namespace UniverseEngine {
    void CmdQueue::ProcessCmdLists(bool wait) {
        while (!this->busyCmdLists.empty()) {
            auto busyCmdList = this->busyCmdLists.front();
            if (busyCmdList.fence->IsComplete()) {
                busyCmdList.cmdList->Reset();
                this->idleCmdLists.push(busyCmdList.cmdList);
                this->busyCmdLists.pop();
            } else if(!wait) {
                break;
            }
        }
    }

    std::shared_ptr<CmdList> CmdQueue::GetCmdList() {
        if (this->idleCmdLists.empty()) {
            auto cmdList = std::shared_ptr<CmdList>(new CmdList(this->device, *this));
            cmdList->Begin();
            return cmdList;
        } else {
            auto cmdList = this->idleCmdLists.front();
            this->idleCmdLists.pop();
            cmdList->Begin();
            return cmdList;
        }
    }
}  // namespace UniverseEngine