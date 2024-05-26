#include <string>
#include <vector>
#include <MedicalWarehouse.h>
#include "Action.h"
#include "Beneficiary.h"
#include "SupplyRequest.h"
#include "Volunteer.h"
#include <stdexcept>
#include <cstring>
#include <stdexcept>
#include <iostream>

using namespace std;

// CoreAction
CoreAction::CoreAction() : errorMsg(""), status(ActionStatus::ERROR) {}
ActionStatus CoreAction::getStatus() const { return status; }
void CoreAction::complete() { status = ActionStatus::COMPLETED; }
void CoreAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    this->errorMsg = errorMsg;
    std::cout << "Error: " + errorMsg << std::endl;
}
string CoreAction::getErrorMsg() const { return errorMsg; }

// SimulateStep
SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}
void SimulateStep::act(MedicalWareHouse &medWareHouse) {
    for (int i = 0; i < numOfSteps; i++) {
        // Step 1: Assign pending requests to available Inventory Managers
        for (SupplyRequest *supplyRequest : medWareHouse.getPendingRequests()) {
            if (supplyRequest && supplyRequest->getStatus() == RequestStatus::PENDING) {
                for (Volunteer *v : medWareHouse.getVolunteers()) {
                    if (auto *inventoryManager = dynamic_cast<InventoryManagerVolunteer *>(v)) {
                        if (inventoryManager && inventoryManager->canTakeRequest(*supplyRequest)) {
                            inventoryManager->acceptRequest(*supplyRequest);
                            supplyRequest->setStatus(RequestStatus::COLLECTING);
                            supplyRequest->setInventoryManagerId(inventoryManager->getId());
                            medWareHouse.moveRequestToInProcess(supplyRequest);
                            break;
                        }
                    }
                }
            }
        }

        // Step 2: Process all Inventory Managers collecting
        for (Volunteer *volunteer : medWareHouse.getVolunteers()) {
            if (InventoryManagerVolunteer *invManager = dynamic_cast<InventoryManagerVolunteer *>(volunteer)) {
                invManager->step();
            }
        }

        // Step 3: Assign collecting requests to available Couriers if Inventory Managers have finished collecting
        for (SupplyRequest *supplyRequest : medWareHouse.getInProcessRequests()) {
            if (supplyRequest && supplyRequest->getStatus() == RequestStatus::COLLECTING) {
                Volunteer &inventoryManager = medWareHouse.getVolunteer(supplyRequest->getInventoryManagerId());
                if (!inventoryManager.isBusy()) {
                    for (Volunteer *v : medWareHouse.getVolunteers()) {
                        if (auto *courier = dynamic_cast<CourierVolunteer *>(v)) {
                            if (courier && courier->canTakeRequest(*supplyRequest)) {
                                courier->acceptRequest(*supplyRequest);
                                supplyRequest->setStatus(RequestStatus::ON_THE_WAY);
                                supplyRequest->setCourierId(courier->getId());
                                break;
                            }
                        }
                    }
                }
            }
        }

        // Step 4: Process all Couriers
        for (Volunteer *volunteer : medWareHouse.getVolunteers()) {
            if (CourierVolunteer *courier = dynamic_cast<CourierVolunteer *>(volunteer)) {
                courier->step();
            }
        }

        // Step 5: Check for completed requests and update their status
        for (Volunteer *v : medWareHouse.getVolunteers()) {
            if (v && v->hasFinishedRequest()) {
                int activeRequestId = v->getActiveRequestId();
                if (activeRequestId != NO_REQUEST) {
                    SupplyRequest &supplyRequest = medWareHouse.getRequest(activeRequestId);
                    if (supplyRequest.getStatus() == RequestStatus::ON_THE_WAY) {
                        supplyRequest.setStatus(RequestStatus::DONE);
                        medWareHouse.moveRequestToCompleted(&supplyRequest);
                    }
                    v->setNoActiveRequest();
                }
            }
        }
    }
    complete();
    medWareHouse.addAction(this);
}

std::string SimulateStep::toString() const { return "step " + std::to_string(numOfSteps); }
SimulateStep *SimulateStep::clone() const { return new SimulateStep(*this); }

// AddRequset
AddRequset::AddRequset(int beneficiaryId) : beneficiaryId(beneficiaryId) {}
void AddRequset::act(MedicalWareHouse &medWareHouse)
{
    if (medWareHouse.isBeneficiaryRegistered(beneficiaryId))
    {
        Beneficiary &beneficiary = medWareHouse.getBeneficiary(beneficiaryId);
        if (beneficiary.canMakeRequest())
        {
            int requestID = medWareHouse.getNextRequestID();
            SupplyRequest *supplyRequest = new SupplyRequest(requestID, beneficiaryId, beneficiary.getBeneficiaryDistance());
            auto result = beneficiary.addRequest(requestID);
            medWareHouse.addRequest(supplyRequest);
            if (result == -1)
            {
                error("Cannot place this request");
                medWareHouse.addAction(this);
                return;
            }
            complete();
            medWareHouse.addAction(this);
            return;
        }
    }
            error("Cannot place this request");
            medWareHouse.addAction(this);
            return;
}

// HELP: I'm not sure about this function
string AddRequset::toString() const { return "request " + std::to_string(beneficiaryId); }
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
        complete();
    }
    medWareHouse.addAction(this);
}

RegisterBeneficiary *RegisterBeneficiary::clone() const { return new RegisterBeneficiary(*this); }
string RegisterBeneficiary::toString() const { return "registerBeneficiary"; }

// PrintRequestStatus
PrintRequestStatus::PrintRequestStatus(int requestId) : requestId(requestId) {}
void PrintRequestStatus::act(MedicalWareHouse &medWareHouse)
{
    try
    {
        SupplyRequest request = medWareHouse.getRequest(requestId);
        std::cout << request.toString() << std::endl;
        complete();
    }
    catch (const std::exception &e)
    {
        error("Request does not exist.");
    }
    medWareHouse.addAction(this);
}
PrintRequestStatus *PrintRequestStatus::clone() const { return new PrintRequestStatus(*this); }
string PrintRequestStatus::toString() const { return "requestStatus " + std::to_string(requestId); }

// PrintBeneficiaryStatus
PrintBeneficiaryStatus::PrintBeneficiaryStatus(int beneficiaryId) : beneficiaryId(beneficiaryId) {}
void PrintBeneficiaryStatus::act(MedicalWareHouse &medWareHouse)
{
    try{
        Beneficiary &ben = medWareHouse.getBeneficiary(beneficiaryId);
        std::cout << "Beneficiary ID: " << beneficiaryId << std::endl;
        for (int requestId : ben.getRequestsIds())
        {
            SupplyRequest &request = medWareHouse.getRequest(requestId);
            std::cout << "Request ID: " << requestId << std::endl;
            std::cout << "Request Status: " << request.getStatusString() << std::endl;
        }
        std::cout << "Requests Left " << ben.getMaxRequests() - ben.getNumRequests() << "\n";
    }
    catch(const std::exception &e)
    {
        error(".");
    }
    medWareHouse.addAction(this);
}
PrintBeneficiaryStatus *PrintBeneficiaryStatus::clone() const { return new PrintBeneficiaryStatus(*this); }
string PrintBeneficiaryStatus::toString() const { return "beneficiaryStatus" + std::to_string(beneficiaryId); }

// PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int volunteerId) : volunteerId(volunteerId) {}
void PrintVolunteerStatus::act(MedicalWareHouse &medWareHouse)
{
    Volunteer &vol = medWareHouse.getVolunteer(volunteerId);
    std::cout << vol.toString() << std::endl;
    complete();
    medWareHouse.addAction(this);
}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const { return new PrintVolunteerStatus(*this); }
string PrintVolunteerStatus::toString() const { return "volunteerStatus " + std::to_string(volunteerId); }

// PrintActionsLog
PrintActionsLog::PrintActionsLog() {}
void PrintActionsLog::act(MedicalWareHouse &medWareHouse)
{
    string s;
    for (CoreAction *action : medWareHouse.getActions())
    {
        if (action->getStatus() == ActionStatus::COMPLETED)
            s = " COMPLETED";
        else
            s = " ERROR" + getErrorMsg();
        std::cout << action->toString() + s + '\n';
    }
    complete();
    medWareHouse.addAction(this);
}
PrintActionsLog *PrintActionsLog::clone() const { return new PrintActionsLog(*this); }
string PrintActionsLog::toString() const { return "log"; }

// Close
Close::Close() {}
void Close::act(MedicalWareHouse &medWareHouse)
{
    for (SupplyRequest *request : medWareHouse.getPendingRequests())
    {
        std::cout << "Request ID: " << request->getId() 
        << ", Beneficiary ID: " <<request->getBeneficiaryId() 
        << ", "<< "Status: " <<request->getStatusString() << std::endl;
    }

    for (SupplyRequest *request : medWareHouse.getInProcessRequests())
    {
        std::cout << "Request ID: " << request->getId() 
        << ", Beneficiary ID: " <<request->getBeneficiaryId() 
        << ", " << "Status: " << request->getStatusString() << std::endl;
    }
    for (SupplyRequest *request : medWareHouse.getCompletedRequests())
    {
        std::cout << "Request ID: " << request->getId() 
        << ", Beneficiary ID: " <<request->getBeneficiaryId() 
        << ", Status: " << request->getStatusString() << std::endl;
    }
    
    medWareHouse.close();
    complete();
}

Close *Close::clone() const { return new Close(*this); }
string Close::toString() const { return "Close"; }

// BackupWareHouse
BackupWareHouse::BackupWareHouse() {}
void BackupWareHouse::act(MedicalWareHouse &medWareHouse)
{
    extern MedicalWareHouse *backup;
    if (backup != nullptr)
    {
        delete (backup);
    }
    backup = new MedicalWareHouse(medWareHouse);
    complete();
    medWareHouse.addAction(this);
}
BackupWareHouse *BackupWareHouse::clone() const { return new BackupWareHouse(*this); }
string BackupWareHouse::toString() const { return "backup"; }

// RestoreWareHouse
RestoreWareHouse::RestoreWareHouse() {}
extern MedicalWareHouse *backup;

void RestoreWareHouse::act(MedicalWareHouse &medWareHouse)
{
    if (backup == nullptr)
    {
        error("Backup does not exist");
        medWareHouse.addAction(this);
    }
    else
    {
        medWareHouse = *backup;
        complete();
        medWareHouse.addAction(this);
    }
}
RestoreWareHouse *RestoreWareHouse::clone() const { return new RestoreWareHouse(*this); }
string RestoreWareHouse::toString() const { return "restore"; }
