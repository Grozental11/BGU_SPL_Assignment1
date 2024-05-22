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
            if (type2 == "manager")
            {
                type = "inventory manager";
            }
            else
            {
                if (type1 == "courier")
                    type = "courier";
            } // need to add checks for invalid input

            std::cout << "Name: " << name << " type: " << type << std::endl;
            if (type == "inventory manager")
            {
                iss >> coolDown;
                InventoryManagerVolunteer *inventoryManagerVolunteer = new InventoryManagerVolunteer(getNextVolunteerId(), name, coolDown);
                volunteers.push_back(inventoryManagerVolunteer);
                std::cout << "Adding inventory manager: " << inventoryManagerVolunteer->getId() << ", " << inventoryManagerVolunteer->getName() << std::endl;
            }
            else if (type == "courier")
            {
                iss >> maxDistance >> distancePerStep;
                CourierVolunteer *courierVolunteer = new CourierVolunteer(getNextVolunteerId(), name, maxDistance, distancePerStep);
                volunteers.push_back(courierVolunteer);
                std::cout << "Adding courier: " << courierVolunteer->getId() << ", " << courierVolunteer->getName() << std::endl;
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
                    throw invalid_argument("Invalid beneficiary ID.");
                }
                AddRequset *requestAction = new AddRequset(beneficiaryId);
                requestAction->act(*this);
                // addAction(requestAction);
            }
            else if (actionType == "step")
            {
                int steps;
                iss >> steps;
                if (iss.fail() || steps <= 0)
                {
                    throw invalid_argument("Invalid number of steps.");
                }
                SimulateStep *stepAction = new SimulateStep(steps);
                stepAction->act(*this);
                addAction(stepAction);
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
                addAction(registerAction);
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
                // addAction(printRequestStatusAction);
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
                addAction(printBeneficiaryStatusAction);
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
                addAction(printActionsLogAction);
            }
            else if (actionType == "close")
            {
                Close *closeAction = new Close();
                closeAction->act(*this);
                addAction(closeAction);
                break; // Exit the loop
            }
            else if (actionType == "backup")
            {
                BackupWareHouse *backupAction = new BackupWareHouse();
                backupAction->act(*this);
                addAction(backupAction);
            }
            else if (actionType == "restore")
            {
                RestoreWareHouse *restoreAction = new RestoreWareHouse();
                restoreAction->act(*this);
                addAction(restoreAction);
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
void MedicalWareHouse::addRequest(SupplyRequest *request) { pendingRequests.push_back(request); }
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
            std::cout << "Found beneficiary" << std::endl;
            return *beneficiary;
        }
    }
    throw std::runtime_error("Beneficiary not found");
}

Volunteer &MedicalWareHouse::getVolunteer(int volunteerId) const
{
    for (Volunteer *volunteer : volunteers)
    {
        // std::cout << volunteer << std::endl;
        // std::cout << "Volunteer ID - " << volunteer->getId() << std::endl;
        if (volunteer->getId() == volunteerId)
        {
            std::cout << "Found Volunteer in MedicalWareHouse" << std::endl;
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
