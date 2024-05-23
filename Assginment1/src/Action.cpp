#include <string>
#include <vector>
#include "MedicalWarehouse.h"
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
CoreAction::CoreAction() : status(ActionStatus::ERROR) {}
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

void SimulateStep::act(MedicalWareHouse &medWareHouse)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        std::cout << "Step " << i + 1 << std::endl;

        // Step 1: Assign pending requests to available Inventory Managers
        for (SupplyRequest *supplyRequest : medWareHouse.getPendingRequests())
        {
            if (supplyRequest->getStatus() == RequestStatus::PENDING)
            {
                for (Volunteer *v : medWareHouse.getVolunteers())
                {
                    if (auto *inventoryManager = dynamic_cast<InventoryManagerVolunteer *>(v))
                    {
                        if (inventoryManager->canTakeRequest(*supplyRequest))
                        {
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

        // Step 2: Process all volunteers' steps (includes Inventory Managers collecting and Couriers delivering)
        for (Volunteer *v : medWareHouse.getVolunteers())
        {
            v->step();
        }

        // Step 3: Assign collecting requests to available Couriers if Inventory Managers have finished collecting
        for (SupplyRequest *supplyRequest : medWareHouse.getInProcessRequests())
        {
            if (supplyRequest->getStatus() == RequestStatus::COLLECTING)
            {
                Volunteer &inventoryManager = medWareHouse.getVolunteer(supplyRequest->getInventoryManagerId());
                if (!inventoryManager.isBusy())
                {
                    for (Volunteer *v : medWareHouse.getVolunteers())
                    {
                        if (auto *courier = dynamic_cast<CourierVolunteer *>(v))
                        {
                            if (courier->canTakeRequest(*supplyRequest))
                            {
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

        // Step 4: Check for completed requests and update their status
        for (Volunteer *v : medWareHouse.getVolunteers())
        {
            if (v->hasFinishedRequest())
            {
                int activeRequestId = v->getActiveRequestId();
                if (activeRequestId != NO_REQUEST)
                {
                    SupplyRequest &supplyRequest = medWareHouse.getRequest(activeRequestId);
                    if (supplyRequest.getStatus() == RequestStatus::ON_THE_WAY)
                    {
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

// void SimulateStep::act(MedicalWareHouse &medWareHouse)
// {
//     for (int i = 0; i < numOfSteps; i++)
//     {
//         std::cout << "Step " << i + 1 << std::endl;

//         // Step 1: Assign pending requests to available Inventory Managers
//         for (SupplyRequest *supplyRequest : medWareHouse.getPendingRequests())
//         {
//             if (supplyRequest->getStatus() == RequestStatus::PENDING)
//             {
//                 for (Volunteer *v : medWareHouse.getVolunteers())
//                 {
//                     if (auto *inventoryManager = dynamic_cast<InventoryManagerVolunteer *>(v))
//                     {
//                         if (inventoryManager->canTakeRequest(*supplyRequest))
//                         {
//                             inventoryManager->acceptRequest(*supplyRequest);
//                             supplyRequest->setStatus(RequestStatus::COLLECTING);
//                             supplyRequest->setInventoryManagerId(inventoryManager->getId());
//                             medWareHouse.moveRequestToInProcess(supplyRequest);
//                             break;
//                         }
//                     }
//                 }
//             }
//         }

//         // Step 2: Assign collecting requests to available Couriers
//         for (SupplyRequest *supplyRequest : medWareHouse.getInProcessRequests())
//         {
//             if (supplyRequest->getStatus() == RequestStatus::COLLECTING)
//             {
//                 for (Volunteer *v : medWareHouse.getVolunteers())
//                 {
//                     if (auto *courier = dynamic_cast<CourierVolunteer *>(v))
//                     {
//                         if (courier->canTakeRequest(*supplyRequest))
//                         {
//                             courier->acceptRequest(*supplyRequest);
//                             supplyRequest->setStatus(RequestStatus::ON_THE_WAY);
//                             supplyRequest->setCourierId(courier->getId());
//                             break;
//                         }
//                     }
//                 }
//             }
//         }

//         // Step 3: Process all volunteers' steps
//         for (Volunteer *v : medWareHouse.getVolunteers())
//         {
//             v->step();
//         }

//         // Step 4: Check for completed requests and update their status
//         for (Volunteer *v : medWareHouse.getVolunteers())
//         {
//             if (v->hasFinishedRequest())
//             {
//                 int activeRequestId = v->getActiveRequestId();
//                 if (activeRequestId != NO_REQUEST)
//                 {
//                     SupplyRequest &supplyRequest = medWareHouse.getRequest(activeRequestId);
//                     if (supplyRequest.getStatus() == RequestStatus::ON_THE_WAY)
//                     {
//                         supplyRequest.setStatus(RequestStatus::DONE);
//                         medWareHouse.moveRequestToCompleted(&supplyRequest);
//                     }
//                     v->setNoActiveRequest();
//                 }
//             }
//         }
//     }
//     complete();
//     medWareHouse.addAction(this);
// }

// ATT: I'm not sure about the return value
std::string SimulateStep::toString() const { return "SimulateStep"; }
SimulateStep *SimulateStep::clone() const { return new SimulateStep(*this); }

// AddRequset
AddRequset::AddRequset(int beneficiaryId) : beneficiaryId(beneficiaryId) {}
void AddRequset::act(MedicalWareHouse &medWareHouse)
{
    Beneficiary &beneficiary = medWareHouse.getBeneficiary(beneficiaryId);
    if (beneficiary.canMakeRequest())
    {
        int requestID = medWareHouse.getNextRequestID();
        std::cout << requestID << std::endl;
        SupplyRequest *supplyRequest = new SupplyRequest(requestID, beneficiaryId, beneficiary.getBeneficiaryDistance());
        std::cout << supplyRequest << std::endl;
        auto result = beneficiary.addRequest(requestID);
        medWareHouse.addRequest(supplyRequest);
        if (result == -1)
        {
            error("Beneficiary reached max requests");
            medWareHouse.addAction(this);
            return;
        }
    }
    else
    {
        error("Beneficiary not found");
        medWareHouse.addAction(this);
        return;
    }
    complete();
    medWareHouse.addAction(this);

    std::cout << "Request added successfully" << std::endl;
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
        complete();
    }
    medWareHouse.addAction(this);
}

RegisterBeneficiary *RegisterBeneficiary::clone() const { return new RegisterBeneficiary(*this); }
string RegisterBeneficiary::toString() const { return "RegisterBeneficiary"; }

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
        std::cerr << "Error: " << e.what() << std::endl;
        this->error("Request does not exist.");
    }
    medWareHouse.addAction(this);
}
PrintRequestStatus *PrintRequestStatus::clone() const { return new PrintRequestStatus(*this); }
string PrintRequestStatus::toString() const { return "PrintRequestStatus"; }

// PrintBeneficiaryStatus
PrintBeneficiaryStatus::PrintBeneficiaryStatus(int beneficiaryId) : beneficiaryId(beneficiaryId) {}
void PrintBeneficiaryStatus::act(MedicalWareHouse &medWareHouse)
{
    std::cout << "PrintBeneficiaryStatus::act " << beneficiaryId << std::endl;
    Beneficiary &ben = medWareHouse.getBeneficiary(beneficiaryId);
    std::cout << ben.toString() << std::endl;
    complete();
    medWareHouse.addAction(this);
}
PrintBeneficiaryStatus *PrintBeneficiaryStatus::clone() const { return new PrintBeneficiaryStatus(*this); }
string PrintBeneficiaryStatus::toString() const { return "PrintBeneficiaryStatus"; }

// PrintVolunteerStatus
PrintVolunteerStatus::PrintVolunteerStatus(int volunteerId) : volunteerId(volunteerId) {}
void PrintVolunteerStatus::act(MedicalWareHouse &medWareHouse)
{
    // std::cout << "PrintVolunteerStatus::act " << volunteerId << std::endl;
    Volunteer &vol = medWareHouse.getVolunteer(volunteerId);
    // std::cout << "Found Volunteer" << std::endl;
    std::cout << vol.toString() << std::endl;
    complete();
    medWareHouse.addAction(this);
}
PrintVolunteerStatus *PrintVolunteerStatus::clone() const { return new PrintVolunteerStatus(*this); }
string PrintVolunteerStatus::toString() const { return "PrintVolunteerStatus"; }

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
string PrintActionsLog::toString() const { return "PrintActionsLog"; }

// Close
Close::Close() {}
void Close::act(MedicalWareHouse &medWareHouse)
{
    std::cout << "Closing warehouse and printing all requests status..." << std::endl;

    // Print the status of all pending requests
    for (SupplyRequest *request : medWareHouse.getPendingRequests())
    {
        std::cout << request->toString() << std::endl;
    }

    // Print the status of all in-process requests
    for (SupplyRequest *request : medWareHouse.getInProcessRequests())
    {
        std::cout << request->toString() << std::endl;
    }

    // Print the status of all completed requests
    for (SupplyRequest *request : medWareHouse.getCompletedRequests())
    {
        std::cout << request->toString() << std::endl;
    }

    // Free all allocated memory for requests
    for (SupplyRequest *request : medWareHouse.getPendingRequests())
    {
        delete request;
    }
    for (SupplyRequest *request : medWareHouse.getInProcessRequests())
    {
        delete request;
    }
    for (SupplyRequest *request : medWareHouse.getCompletedRequests())
    {
        delete request;
    }

    // Clear the request vectors
    medWareHouse.getPendingRequests().clear();
    medWareHouse.getInProcessRequests().clear();
    medWareHouse.getCompletedRequests().clear();

    // Set the warehouse status to closed
    medWareHouse.close();

    // Free all allocated memory for volunteers
    for (Volunteer *volunteer : medWareHouse.getVolunteers())
    {
        delete volunteer;
    }

    // Clear the volunteers vector
    medWareHouse.getVolunteers().clear();

    // // Free all allocated memory for beneficiaries
    // for (Beneficiary *beneficiary : medWareHouse.getBeneficiaries())
    // {
    //     delete beneficiary;
    // }

    // // Clear the beneficiaries vector
    // medWareHouse.getBeneficiaries().clear();

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
string BackupWareHouse::toString() const { return "BackupWareHouse"; }

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
string RestoreWareHouse::toString() const { return "RestoreWareHouse"; }
