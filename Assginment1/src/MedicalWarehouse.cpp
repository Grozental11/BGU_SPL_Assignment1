#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

#include "SupplyRequest.h"
#include "Beneficiary.h"
#include <MedicalWarehouse.h>
#include "Volunteer.h"
#include <Action.h>

using namespace std;

MedicalWareHouse::MedicalWareHouse(const string &configFilePath)
    : isOpen(true), actionsLog(), volunteers(), pendingRequests(), inProcessRequests(), completedRequests(), Beneficiaries(0), beneficiaryCounter(-1), volunteerCounter(-1), nextRequestID(0)
{
    initializeFromConfig(configFilePath);
}
// Role of 5 functions
// Destructor
MedicalWareHouse ::~MedicalWareHouse()
{
    for (size_t i = 0; i < actionsLog.size(); ++i) {
        try
        {
            delete actionsLog[i];
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    actionsLog.clear();

    for (size_t i = 0; i < volunteers.size(); ++i) {
        std::cout << "Deleting volunteers" << std::endl;
        delete volunteers[i];
    }
    volunteers.clear();

    for (size_t i = 0; i < pendingRequests.size(); ++i) {
        std::cout << "Deleting pendingRequests" << std::endl;
        delete pendingRequests[i];
    }
    pendingRequests.clear();

    for (size_t i = 0; i < inProcessRequests.size(); ++i) {
        delete inProcessRequests[i];
    }
    inProcessRequests.clear();

    for (size_t i = 0; i < completedRequests.size(); ++i) {
        delete completedRequests[i];
    }
    completedRequests.clear();

    for (size_t i = 0; i < Beneficiaries.size(); ++i) {
        delete Beneficiaries[i];
    }
    Beneficiaries.clear();
}
// Copy Assignment Operator
MedicalWareHouse &MedicalWareHouse::operator=(const MedicalWareHouse &other){
    if (this == &other)
    {
        return *this;
    }
    isOpen = other.isOpen;
    beneficiaryCounter = other.beneficiaryCounter;
    volunteerCounter = other.volunteerCounter;
    nextRequestID = other.nextRequestID;

    for (auto &beneficiary : Beneficiaries)
    {
        delete beneficiary;
    }
    Beneficiaries.clear();
    for (auto &beneficiary : other.Beneficiaries)
    {
        Beneficiaries.push_back(beneficiary->clone());
    }

    for (auto &volunteer : volunteers)
    {
        delete volunteer;
    }
    volunteers.clear();
    for (auto &volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer->clone());
    }

    for (auto &request : pendingRequests)
    {
        delete request;
    }
    pendingRequests.clear();
    for (auto &request : other.pendingRequests)
    {
        pendingRequests.push_back(request->clone());
    }

    for (auto &request : inProcessRequests)
    {
        delete request;
    }
    inProcessRequests.clear();
    for (auto &request : other.inProcessRequests)
    {
        inProcessRequests.push_back(request->clone());
    }

    for (auto &request : completedRequests)
    {
        delete request;
    }
    completedRequests.clear();
    for (auto &request : other.completedRequests)
    {
        completedRequests.push_back(request->clone());
    }

    for (auto &action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();
    for (auto &action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }

    return *this;

}
// Copy Constructor
MedicalWareHouse::MedicalWareHouse(const MedicalWareHouse &other)
    : isOpen(other.isOpen), actionsLog(), volunteers(), pendingRequests(), inProcessRequests(), completedRequests(), Beneficiaries(0), beneficiaryCounter(other.beneficiaryCounter), volunteerCounter(other.volunteerCounter), nextRequestID(other.nextRequestID)
{
    for (auto &beneficiary : other.Beneficiaries)
    {
        Beneficiaries.push_back(beneficiary->clone());
    }

    for (auto &volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer->clone());
    }

    for (auto &request : other.pendingRequests)
    {
        pendingRequests.push_back(request->clone());
    }

    for (auto &request : other.inProcessRequests)
    {
        inProcessRequests.push_back(request->clone());
    }

    for (auto &request : other.completedRequests)
    {
        completedRequests.push_back(request->clone());
    }

    for (auto &action : other.actionsLog)
    {
        actionsLog.push_back(action->clone());
    }
}
// Move Constructor
MedicalWareHouse::MedicalWareHouse(MedicalWareHouse &&other)
    : isOpen(other.isOpen), actionsLog(other.actionsLog), volunteers(other.volunteers), pendingRequests(other.pendingRequests), inProcessRequests(other.inProcessRequests), completedRequests(other.completedRequests), Beneficiaries(other.Beneficiaries), beneficiaryCounter(other.beneficiaryCounter), volunteerCounter(other.volunteerCounter), nextRequestID(other.nextRequestID)
{
    other.isOpen = false;
    other.beneficiaryCounter = -1;
    other.volunteerCounter = -1;
    other.nextRequestID = 0;
    other.Beneficiaries.clear();
    other.volunteers.clear();
    other.pendingRequests.clear();
    other.inProcessRequests.clear();
    other.completedRequests.clear();
    other.actionsLog.clear();
}
// Move Assignment Operator
MedicalWareHouse &MedicalWareHouse::operator=(MedicalWareHouse &&other)
{
    if (this == &other)
    {
        return *this;
    }
    isOpen = other.isOpen;
    beneficiaryCounter = other.beneficiaryCounter;
    volunteerCounter = other.volunteerCounter;
    nextRequestID = other.nextRequestID;

    for (auto &beneficiary : Beneficiaries)
    {
        delete beneficiary;
    }
    Beneficiaries.clear();
    for (auto &beneficiary : other.Beneficiaries)
    {
        Beneficiaries.push_back(beneficiary);
    }
    other.Beneficiaries.clear();

    for (auto &volunteer : volunteers)
    {
        delete volunteer;
    }
    volunteers.clear();
    for (auto &volunteer : other.volunteers)
    {
        volunteers.push_back(volunteer);
    }
    other.volunteers.clear();

    for (auto &request : pendingRequests)
    {
        delete request;
    }
    pendingRequests.clear();
    for (auto &request : other.pendingRequests)
    {
        pendingRequests.push_back(request);
    }
    other.pendingRequests.clear();

    for (auto &request : inProcessRequests)
    {
        delete request;
    }
    inProcessRequests.clear();
    for (auto &request : other.inProcessRequests)
    {
        inProcessRequests.push_back(request);
    }
    other.inProcessRequests.clear();

    for (auto &request : completedRequests)
    {
        delete request;
    }
    completedRequests.clear();
    for (auto &request : other.completedRequests)
    {
        completedRequests.push_back(request);
    }
    other.completedRequests.clear();

    for (auto &action : actionsLog)
    {
        delete action;
    }
    actionsLog.clear();
    for (auto &action : other.actionsLog)
    {
        actionsLog.push_back(action);
    }
    other.actionsLog.clear();

    return *this;
}

void MedicalWareHouse::initializeFromConfig(const std::string &configFilePath)
{
    std::ifstream inFile(configFilePath);
    if (!inFile.is_open())
    {
        std::cerr << "Error: Cannot open configuration file." << std::endl;
        return;
    }

    std::string line, firstWord, name, type;
    int coolDown, maxRequests, distance, maxDistance, distancePerStep;
    while (getline(inFile, line))
    {
        std::istringstream iss(line);
        iss >> firstWord;
        if (firstWord == "beneficiary")
        {
            iss >> name >> type >> distance >> maxRequests;
            if (type == "hospital")
            {
                HospitalBeneficiary *hospitalBeneficiary = new HospitalBeneficiary(getNextBeneficiaryId(), name, distance, maxRequests);
                addNewBeneficiary(hospitalBeneficiary);
                std::cout << "Adding hospital beneficiary: " << hospitalBeneficiary->getId() << ", " << hospitalBeneficiary->getName() << std::endl;
            }
            else if (type == "clinic")
            {
                ClinicBeneficiary *clinicBeneficiary = new ClinicBeneficiary(getNextBeneficiaryId(), name, distance, maxRequests);
                addNewBeneficiary(clinicBeneficiary);
                std::cout << "Adding clinic beneficiary: " << clinicBeneficiary->getId() << ", " << clinicBeneficiary->getName() << std::endl;
            }
        }
        else if (firstWord == "volunteer")
        {
            // volunteer Monica inventory manager 2 4
            // volunteer Chandler courier 7 4
            std::string type1, type2;
            iss >> name >> type1 >> type2;
            if (type1 == "inventory")
            {
                type = "inventory manager";
            }
            else
            {
                if (type1 == "courier")
                {
                    type = "courier";
                }
            } // need to add checks for invalid input

            // std::cout << "Name: " << name << " type: " << type << std::endl;
            if (type == "inventory manager")
            {
                iss >> coolDown;
                std::cout << "Cool Down: " << coolDown << std::endl;
                InventoryManagerVolunteer *inventoryManagerVolunteer = new InventoryManagerVolunteer(getNextVolunteerId(), name, coolDown);
                volunteers.push_back(inventoryManagerVolunteer);
                std::cout << "Adding inventory manager: " << inventoryManagerVolunteer -> toString() << std::endl;
                // std::cout << "Adding inventory manager: " << inventoryManagerVolunteer->getId() << ", " << inventoryManagerVolunteer->getName() << std::endl;
            }
            else if (type == "courier")
            {
                maxDistance = std::stoi(type2); // Convert type2 to an integer
                iss >> distancePerStep;
                //std::cout << "Max Distance: " << maxDistance << " Distance Per Step: " << distancePerStep << std::endl;
                CourierVolunteer *courierVolunteer = new CourierVolunteer(getNextVolunteerId(), name, maxDistance, distancePerStep);
                volunteers.push_back(courierVolunteer);
                std::cout << "Adding courier: " << courierVolunteer -> toString() << std::endl;
                // std::cout << "Adding courier: " << courierVolunteer->getId() << ", " << courierVolunteer->getName() << courierVolunteer->getDistancePerStep() << std::endl;
            }
        }
        firstWord.clear();
        name.clear();
        type.clear();
        coolDown = -1;
        maxRequests = -1;
        distance = -1;
        maxDistance = -1;
        distancePerStep = -1;
    }
    inFile.close();
}
void MedicalWareHouse::start()
{
    cout << "Medical services are now open!" << endl;

    string command;
    while (true)
    {
        cout << "> ";
        getline(cin, command);
        if (command.empty())
        {
            continue;
        }

        istringstream iss(command);
        string actionType;
        iss >> actionType;

        try
        {
            if (actionType == "request")
            {
                int beneficiaryId;
                iss >> beneficiaryId;
                if (iss.fail())
                {
                    throw invalid_argument("Cannot place this request");
                }
                AddRequset *requestAction = new AddRequset(beneficiaryId);
                requestAction->act(*this);
            }
            else if (actionType == "step")
            {
                std::cout << "Simulating step" << std::endl;
                int steps;
                iss >> steps;
                if (iss.fail() || steps <= 0)
                {
                    throw invalid_argument("Invalid number of steps.");
                }
                SimulateStep *stepAction = new SimulateStep(steps);
                stepAction->act(*this);
            }
            else if (actionType == "register")
            {
                string name, type;
                int distance, maxRequests;
                iss >> name >> type >> distance >> maxRequests;
                if (iss.fail())
                {
                    throw invalid_argument("Invalid register command.");
                }
                RegisterBeneficiary *registerAction = new RegisterBeneficiary(name, type, distance, maxRequests);
                registerAction->act(*this);
            }
            else if (actionType == "requestStatus")
            {
                int requestId;
                iss >> requestId;
                if (iss.fail())
                {
                    throw invalid_argument("Invalid request ID.");
                }
                PrintRequestStatus *printRequestStatusAction = new PrintRequestStatus(requestId);
                printRequestStatusAction->act(*this);
            }
            else if (actionType == "beneficiaryStatus")
            {
                int beneficiaryId;
                iss >> beneficiaryId;
                if (iss.fail())
                {
                    throw invalid_argument("Invalid beneficiary ID.");
                }
                PrintBeneficiaryStatus *printBeneficiaryStatusAction = new PrintBeneficiaryStatus(beneficiaryId);
                printBeneficiaryStatusAction->act(*this);
            }
            else if (actionType == "volunteerStatus")
            {
                int volunteerId;
                iss >> volunteerId;
                if (iss.fail()) // need to add more checks
                {
                    throw invalid_argument("Invalid volunteer ID.");
                }
                PrintVolunteerStatus *printVolunteerStatusAction = new PrintVolunteerStatus(volunteerId);
                printVolunteerStatusAction->act(*this);
                // addAction(printVolunteerStatusAction);
            }
            else if (actionType == "log")
            {
                PrintActionsLog *printActionsLogAction = new PrintActionsLog();
                printActionsLogAction->act(*this);
            }
            else if (actionType == "close")
            {
                Close *closeAction = new Close();
                closeAction->act(*this);
                delete closeAction;
                break; // Exit the loop
            }
            else if (actionType == "backup")
            {
                BackupWareHouse *backupAction = new BackupWareHouse();
                backupAction->act(*this);
            }
            else if (actionType == "restore")
            {
                RestoreWareHouse *restoreAction = new RestoreWareHouse();
                restoreAction->act(*this);
            }
            else
            {
                throw invalid_argument("Unknown command.");
            }
        }
        catch (const exception &e)
        {
            cerr << "Error: " << e.what() << endl;
        }
    }
}
void MedicalWareHouse::addRequest(SupplyRequest *request)
{
    if (request->getStatus() == RequestStatus::PENDING)
        pendingRequests.push_back(request);
    else if (request->getStatus() == RequestStatus::COLLECTING)
        inProcessRequests.push_back(request);
    else if (request->getStatus() == RequestStatus::DONE)
        completedRequests.push_back(request);
    else
        throw std::runtime_error("Unable to add request to the warehouse. Invalid status.");
}
void MedicalWareHouse::addAction(CoreAction *action)
{
    actionsLog.push_back(action);
}
Beneficiary &MedicalWareHouse::getBeneficiary(int beneficiaryId) const
{
    for (auto &beneficiary : Beneficiaries)
    {
        if (beneficiary->getId() == beneficiaryId)
        {
            return *beneficiary;
        }
    }
    throw std::runtime_error("Beneficiary not found");
}
Volunteer &MedicalWareHouse::getVolunteer(int volunteerId) const
{
    for (Volunteer *volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    throw std::runtime_error("Volunteer not found");
}
SupplyRequest &MedicalWareHouse::getRequest(int requestId) const
{
    for (SupplyRequest *o : pendingRequests)
        if (o->getId() == requestId)
            return *o;
    for (SupplyRequest *o : inProcessRequests)
        if (o->getId() == requestId)
            return *o;
    for (SupplyRequest *o : completedRequests)
        if (o->getId() == requestId)
            return *o;

    throw std::runtime_error("Request not found");
}
const vector<CoreAction *> &MedicalWareHouse::getActions() const { return actionsLog; }
void MedicalWareHouse::close() { isOpen = false; }
void MedicalWareHouse::open() { isOpen = true; }

// Added Functions
int MedicalWareHouse::getBeneficiaryCounter() const { return beneficiaryCounter; }
int MedicalWareHouse::getNextBeneficiaryId()
{
    ++beneficiaryCounter;
    return beneficiaryCounter;
}
void MedicalWareHouse::addNewBeneficiary(Beneficiary *beneficiary)
{
    Beneficiaries.push_back(beneficiary);
    std::cout << "Beneficiarie added" << std::endl;
}
int MedicalWareHouse::getVolunteerCounter() { return volunteerCounter; }
int MedicalWareHouse::getNextVolunteerId()
{
    ++volunteerCounter;
    return volunteerCounter;
}
int MedicalWareHouse::getNextRequestID()
{
    return nextRequestID++;
}
std::vector<SupplyRequest *> &MedicalWareHouse::getPendingRequests()
{
    return pendingRequests;
}
std::vector<SupplyRequest *> &MedicalWareHouse::getInProcessRequests()
{
    return inProcessRequests;
}
std::vector<SupplyRequest *> &MedicalWareHouse::getCompletedRequests()
{
    return completedRequests;
}
std::vector<Volunteer *> &MedicalWareHouse::getVolunteers()
{
    return volunteers;
}
bool MedicalWareHouse::isInPending(SupplyRequest *request)
{
    if (std::find(pendingRequests.begin(), pendingRequests.end(), request) != pendingRequests.end())
        return true;
    return false;
}
void MedicalWareHouse::moveRequestToPending(SupplyRequest *request)
{
    if (!isInPending(request))
    {
        pendingRequests.push_back(request);
        auto inProcessIt = std::find(inProcessRequests.begin(), inProcessRequests.end(), request);
        if (inProcessIt != inProcessRequests.end())
        {
            inProcessRequests.erase(inProcessIt);
        }

        auto completedIt = std::find(completedRequests.begin(), completedRequests.end(), request);
        if (completedIt != completedRequests.end())
        {
            completedRequests.erase(completedIt);
        }
        request->setStatus(RequestStatus::PENDING);
    }
}
void MedicalWareHouse::moveRequestToInProcess(SupplyRequest *request)
{
    auto it = std::find(pendingRequests.begin(), pendingRequests.end(), request);
    if (it != pendingRequests.end())
    {
        inProcessRequests.push_back(request);
        pendingRequests.erase(it);
    }
    else
    {
        throw std::runtime_error("Request not found in pending requests");
    }
}
void MedicalWareHouse::moveRequestToCompleted(SupplyRequest *request)
{
    auto it = std::find(inProcessRequests.begin(), inProcessRequests.end(), request);
    if (it != inProcessRequests.end())
    {
        completedRequests.push_back(request);
        inProcessRequests.erase(it);
    }
    else
    {
        throw std::runtime_error("Request not found in in-process requests");
    }
}
void MedicalWareHouse::eraseFromPending(SupplyRequest *request)
{
    if (std::find(pendingRequests.begin(), pendingRequests.end(), request) != pendingRequests.end())
        pendingRequests.erase(std::remove(pendingRequests.begin(), pendingRequests.end(), request), pendingRequests.end());
}
void MedicalWareHouse::setRequest(SupplyRequest *request)
{
    if (std::find(inProcessRequests.begin(), inProcessRequests.end(), request) != inProcessRequests.end())
    {
        if (request->getStatus() == RequestStatus::COLLECTING)
        {
            inProcessRequests.erase(std::remove(inProcessRequests.begin(), inProcessRequests.end(), request), inProcessRequests.end());
            pendingRequests.push_back(request);
        }
        else if (request->getStatus() == RequestStatus::ON_THE_WAY)
        {
            inProcessRequests.erase(std::remove(inProcessRequests.begin(), inProcessRequests.end(), request), inProcessRequests.end());
            completedRequests.push_back(request);
            request->setStatus(RequestStatus::DONE);
        }
    }
}