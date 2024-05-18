#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include "SupplyRequest.h"
#include "Beneficiary.h"
#include <MedicalWarehouse.h>
#include "Volunteer.h"

MedicalWareHouse::MedicalWareHouse(const string &configFilePath)
    : isOpen(true), actionsLog(), volunteers(), pendingRequests(), inProcessRequests(), completedRequests(), Beneficiaries(), beneficiaryCounter(0), volunteerCounter(0) {}

// ATT: NEED TO IMPLEMENT THE START FUNCTION
void MedicalWareHouse::start() {}

void MedicalWareHouse::addRequest(SupplyRequest *request)
{
    // ATT: Do I need to make checks for the request?
    pendingRequests.push_back(request);
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
    for (auto &request : pendingRequests)
    {
        if (request->getId() == requestId)
        {
            return *request;
        }
    }
    for (auto &request : inProcessRequests)
    {
        if (request->getId() == requestId)
        {
            return *request;
        }
    }
    for (auto &request : completedRequests)
    {
        if (request->getId() == requestId)
        {
            return *request;
        }
    }
    throw "Request not found";
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

void MedicalWareHouse::initializeFromConfig(const std::string &configFilePath)
{
    std::ifstream configFile(configFilePath);
    if (!configFile.is_open())
    {
        std::cerr << "Error: Cannot open configuration file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(configFile, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "beneficiary")
        {
            std::string name, facilityType;
            int locationDistance, maxRequests;
            iss >> name >> facilityType >> locationDistance >> maxRequests;
            int id = getNextBeneficiaryId();
            if (facilityType == "hospital")
            {
                Beneficiaries.push_back(new HospitalBeneficiary(id, name, locationDistance, maxRequests));
            }
            else if (facilityType == "clinic")
            {
                Beneficiaries.push_back(new ClinicBeneficiary(id, name, locationDistance, maxRequests));
            }
            else
            {
                std::cerr << "Error: Unknown facility type " << facilityType << std::endl;
            }
        }
        else if (type == "volunteer")
        {
            std::string name, role;
            int cooldownOrMaxDistance, distancePerStep = 0;
            iss >> name >> role >> cooldownOrMaxDistance;
            if (role == "inventory" || role == "inventory_manager")
            {
                volunteers.push_back(new InventoryManagerVolunteer(getNextVolunteerId(), name, cooldownOrMaxDistance));
            }
            else if (role == "courier")
            {
                iss >> distancePerStep;
                volunteers.push_back(new CourierVolunteer(getNextVolunteerId(), name, cooldownOrMaxDistance, distancePerStep));
            }
            else
            {
                std::cerr << "Error: Unknown volunteer role " << role << std::endl;
            }
        }
        else
        {
            std::cerr << "Error: Unknown line type " << type << std::endl;
        }
    }

    configFile.close();
}
