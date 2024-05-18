#include <string>
#include <vector>
#include "MedicalWarehouse.h"
#include "Action.h"
#include "Beneficiary.h"

// CoreAction
CoreAction::CoreAction() : status(ActionStatus::ERROR) {}
ActionStatus CoreAction::getStatus() const { return status; }
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
RegisterBeneficiary::RegisterBeneficiary(const string &beneficiaryName, const string &beneficiaryType, int distance, int maxRequests) {}

void RegisterBeneficiary::act(MedicalWareHouse &medWareHouse)
{
    Beneficiary *beneficiary = nullptr;
    int newId = medWareHouse.getNextBeneficiaryId();

    if (beneficiaryType == "Hospital")
    {
        beneficiary = new HospitalBeneficiary(newId, beneficiaryName, distance, maxRequests);
    }
    else if (beneficiaryType == "Clinic")
    {
        beneficiary = new ClinicBeneficiary(newId, beneficiaryName, distance, maxRequests);
    }
    else
    {
        error("Invalid beneficiary type");
    }

    if (beneficiary)
    {
        medWareHouse.addNewBeneficiary(beneficiary);
    }
}

RegisterBeneficiary *RegisterBeneficiary::clone() const { return new RegisterBeneficiary(*this); }
string RegisterBeneficiary::toString() const { return "RegisterBeneficiary"; }

// PrintRequestStatus
PrintRequestStatus::PrintRequestStatus(int requestId) : requestId(requestId) {}
void PrintRequestStatus::act(MedicalWareHouse &medWareHouse) {}
PrintRequestStatus *PrintRequestStatus::clone() const { return new PrintRequestStatus(*this); }
string PrintRequestStatus::toString() const { return "PrintRequestStatus"; }

// PrintBeneficiaryStatus
PrintBeneficiaryStatus::PrintBeneficiaryStatus(int beneficiaryId) : beneficiaryId(beneficiaryId) {}
void PrintBeneficiaryStatus::act(MedicalWareHouse &medWareHouse) {}
PrintBeneficiaryStatus *PrintBeneficiaryStatus::clone() const { return new PrintBeneficiaryStatus(*this); }
string PrintBeneficiaryStatus::toString() const { return "PrintBeneficiaryStatus"; }

// PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int volunteerId) : volunteerId(volunteerId) {}
void PrintVolunteerStatus::act(MedicalWareHouse &medWareHouse) {}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const { return new PrintVolunteerStatus(*this); }
string PrintVolunteerStatus::toString() const { return "PrintVolunteerStatus"; }

// PrintActionsLog
PrintActionsLog::PrintActionsLog() {}
void PrintActionsLog::act(MedicalWareHouse &medWareHouse) {}
PrintActionsLog *PrintActionsLog::clone() const { return new PrintActionsLog(*this); }
string PrintActionsLog::toString() const { return "PrintActionsLog"; }

// Close
Close::Close() {}
void Close::act(MedicalWareHouse &medWareHouse) {}
Close *Close::clone() const { return new Close(*this); }
string Close::toString() const { return "Close"; }

// BackupWareHouse
BackupWareHouse::BackupWareHouse() {}
void BackupWareHouse::act(MedicalWareHouse &medWareHouse) {}
BackupWareHouse *BackupWareHouse::clone() const { return new BackupWareHouse(*this); }
string BackupWareHouse::toString() const { return "BackupWareHouse"; }

// RestoreWareHouse
RestoreWareHouse::RestoreWareHouse() {}
void RestoreWareHouse::act(MedicalWareHouse &medWareHouse) {}
RestoreWareHouse *RestoreWareHouse::clone() const { return new RestoreWareHouse(*this); }
string RestoreWareHouse::toString() const { return "RestoreWareHouse"; }
