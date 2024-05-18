#include <string>
#include <vector>
#include "MedicalWarehouse.h"
#include "Action.h"
#include "Beneficiary.h"
#include <stdexcept>
#include <cstring>
#include <stdexcept>

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
// ATT: I'm not sure about the return value
std::string SimulateStep::toString() const { return "SimulateStep"; }
SimulateStep *SimulateStep::clone() const { return new SimulateStep(*this); }

// AddRequset
AddRequset::AddRequset(int beneficiaryId) : beneficiaryId(beneficiaryId) {}
void AddRequset::act(MedicalWareHouse &medWareHouse)
{
    Beneficiary *beneficiary = &medWareHouse.getBeneficiary(beneficiaryId);
    if (beneficiary)
    {
        beneficiary->addRequest(beneficiaryId); // continue from here
        complete();
    }
    else
    {
        error("Beneficiary not found");
    }
}

// HELP: I'm not sure about this function
string AddRequset::toString() const { return "AddRequset"; }
AddRequset *AddRequset::clone() const { return new AddRequset(*this); }

// RegisterBeneficiary
beneficiaryType stringToBeneficiaryType(const string &type) // Added function
{
    if (type == "hospital")
    {
        return beneficiaryType::Hospital;
    }
    else if (type == "clinic")
    {
        return beneficiaryType::Clinic;
    }
    else
    {
        throw std::invalid_argument("Invalid beneficiary type");
    }
}

RegisterBeneficiary::RegisterBeneficiary(const string &beneficiaryName, const string &beneficiaryType, int distance, int maxRequests)
    : beneficiaryName(beneficiaryName), beneficiaryType_(stringToBeneficiaryType(beneficiaryType)), distance(distance), maxRequests(maxRequests) {}

void RegisterBeneficiary::act(MedicalWareHouse &medWareHouse)
{
    Beneficiary *beneficiary = nullptr;
    int newId = medWareHouse.getNextBeneficiaryId();

    if (beneficiaryType_ == beneficiaryType::Hospital)
    {
        beneficiary = new HospitalBeneficiary(newId, beneficiaryName, distance, maxRequests);
    }
    else if (beneficiaryType_ == beneficiaryType::Clinic)
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
