#include <string>
#include <vector>
#include "MedicalWarehouse.h"
#include "Action.h"

// CoreAction
CoreAction::CoreAction() : status(ActionStatus::ERROR) {}
CoreAction::ActionStatus CoreAction::getStatus() const { return status; }
void CoreAction::complete() { status = ActionStatus::COMPLETED; }
void CoreAction::error(string errorMsg) { status = ActionStatus::ERROR; }
string CoreAction::getErrorMsg() const { return errorMsg; }

// SimulateStep
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}
void SimulateStep::act(MedicalWareHouse &medWareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        for (auto &action : medWareHouse.getActions())
        {
            action->act(medWareHouse);
        }
    }
    complete();
}
// HELP: I'm not sure about this function
void SimulateStep::act(MedicalWareHouse &medWareHouse) {}
// ATT: I'm not sure about the return value
std::string SimulateStep::toString() const { return "SimulateStep"; }
SimulateStep *SimulateStep::clone() const { return new SimulateStep(*this); }

// AddRequset
AddRequset::AddRequset(int beneficiaryId) : beneficiaryId(beneficiaryId) {}
void AddRequset::act(MedicalWareHouse &medWareHouse) {}
// HELP: I'm not sure about this function
string AddRequset::toString() const { return "AddRequset"; }
AddRequset *AddRequset::clone() const { return new AddRequset(*this); }

// RegisterBeneficiary
RegisterBeneficiary::RegisterBeneficiary(const string &beneficiaryName, const string &beneficiaryType, int distance, int maxRequests)
{
    // Continue the funcion
}

void RegisterBeneficiary::act(MedicalWareHouse &medWareHouse)
{
    // Continue the funcion
}

RegisterBeneficiary *RegisterBeneficiary::clone() const { return new RegisterBeneficiary(*this); }
string RegisterBeneficiary::toString() const { return "RegisterBeneficiary"; }

// PrintRequestStatus
PrintRequestStatus::PrintRequestStatus(int requestId) : requestId(requestId) {}
void PrintRequestStatus::act(MedicalWareHouse &medWareHouse) {}
PrintRequestStatus *PrintRequestStatus::clone() const { return new PrintRequestStatus(*this); }
string PrintRequestStatus::toString() const { return "PrintRequestStatus"; }

// PrintActionStatus