#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "SupplyRequest.h"
#include "Beneficiary.h"
#include <MedicalWarehouse.h>
#include "Volunteer.h"
#include <Action.h>

using namespace std;

MedicalWareHouse::MedicalWareHouse(const string &configFilePath)
    : isOpen(true), actionsLog(), volunteers(), pendingRequests(), inProcessRequests(), completedRequests(), Beneficiaries(), beneficiaryCounter(0), volunteerCounter(0)
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
            }
            else if (type == "clinic")
            {
                ClinicBeneficiary *clinicBeneficiary = new ClinicBeneficiary(getNextBeneficiaryId(), name, distance, maxRequests);
                addNewBeneficiary(clinicBeneficiary);
            }
        }
        else if (firstWord == "volunteer")
        {
            iss >> name >> type;
            if (type == "inventory_manager")
            {
                iss >> coolDown;
                InventoryManagerVolunteer *inventoryManagerVolunteer = new InventoryManagerVolunteer(getNextVolunteerId(), name, coolDown);
                volunteers.push_back(inventoryManagerVolunteer);
            }
            else if (type == "courier")
            {
                iss >> maxDistance >> distancePerStep;
                CourierVolunteer *courierVolunteer = new CourierVolunteer(getNextVolunteerId(), name, maxDistance, distancePerStep);
                volunteers.push_back(courierVolunteer);
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
                // requestAction->act(*this);
                addAction(requestAction);
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
                cout << "IM HERE " << endl;
                int requestId;
                iss >> requestId;
                cout << "requestId: " << requestId << endl;
                if (iss.fail())
                {
                    throw invalid_argument("Invalid request ID.");
                }
                PrintRequestStatus *printRequestStatusAction = new PrintRequestStatus(requestId);
                printRequestStatusAction->act(*this);
                addAction(printRequestStatusAction);
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
                if (iss.fail())
                {
                    throw invalid_argument("Invalid volunteer ID.");
                }
                PrintVolunteerStatus *printVolunteerStatusAction = new PrintVolunteerStatus(volunteerId);
                printVolunteerStatusAction->act(*this);
                addAction(printVolunteerStatusAction);
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
            return *beneficiary;
        }
    }
    throw "Beneficiary not found";
}

Volunteer &MedicalWareHouse::getVolunteer(int volunteerId) const
{
    for (auto &volunteer : volunteers)
    {
        if (volunteer->getId() == volunteerId)
        {
            return *volunteer;
        }
    }
    throw "Volunteer not found";
}
SupplyRequest &MedicalWareHouse::getRequest(int requestId) const
{
    try
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
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}
const vector<CoreAction *> &MedicalWareHouse::getActions() const { return actionsLog; }
void MedicalWareHouse::close() { isOpen = false; }
void MedicalWareHouse::open() { isOpen = true; }

// Added Functions
int MedicalWareHouse::getBeneficiaryCounter() const { return beneficiaryCounter; }
int MedicalWareHouse::getNextBeneficiaryId()
{
    beneficiaryCounter++;
    return beneficiaryCounter;
}

void MedicalWareHouse::addNewBeneficiary(Beneficiary *beneficiary)
{
    Beneficiaries.push_back(beneficiary);
}

int MedicalWareHouse::getVolunteerCounter() { return volunteerCounter; }
int MedicalWareHouse::getNextVolunteerId()
{
    volunteerCounter++;
    return volunteerCounter;
}

int MedicalWareHouse::getNextRequestID()
{
    nextRequestID++;
    return nextRequestID;
}