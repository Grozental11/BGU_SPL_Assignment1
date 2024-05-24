#include <string>
#include <vector>
#include <SupplyRequest.h>
#include <Volunteer.h>
#include <sstream>
#include <iostream>
using std::string;
using std::vector;

// Volunteer implementation
Volunteer::Volunteer(int id, const string &name)
    : id(id), name(name), activeRequestId(NO_REQUEST), completedRequestId(NO_REQUEST) {}
int Volunteer::getId() const
{
    return id;
}
const string &Volunteer::getName() const
{
    return name;
}
int Volunteer::getActiveRequestId() const
{
    return activeRequestId;
}
int Volunteer::getCompletedRequestId() const
{
    return completedRequestId;
}
bool Volunteer::isBusy() const
{
    return activeRequestId != NO_REQUEST;
}
bool Volunteer::hasFinishedRequest()
{
    if (activeRequestId == completedRequestId)
        return true;
    return false;
}

void Volunteer::setActiveRequestId(int activeRequestId)
{
    this->activeRequestId = activeRequestId;
}

// InventoryManagerVolunteer implementation
InventoryManagerVolunteer::InventoryManagerVolunteer(int id, const string &name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown+1), timeLeft(0) {}

InventoryManagerVolunteer *InventoryManagerVolunteer::clone() const
{
    return new InventoryManagerVolunteer(*this);
}
void InventoryManagerVolunteer::step()
{
    if (activeRequestId != NO_REQUEST)
    {
        timeLeft -= 1;
        //std::cout << "(InventoryManagerVolunteer) timeLeft: " << timeLeft << std::endl;
        //std::cout << "(InventoryManagerVolunteer) activeRequestId: " << activeRequestId << std::endl;
        // ATT: Do I need to check if timeLeft is less than 0?
        if (timeLeft == 0)
        {
            completedRequestId = activeRequestId;
            activeRequestId = NO_REQUEST;
        }
        //std::cout << "AFTER (InventoryManagerVolunteer) timeLeft: " << timeLeft << std::endl;
        //std::cout << "AFTER (InventoryManagerVolunteer) activeRequestId: " << activeRequestId << std::endl;
    }
}
int InventoryManagerVolunteer::getCoolDown() const
{
    return coolDown;
}
int InventoryManagerVolunteer::getTimeLeft() const
{
    return timeLeft;
}
bool InventoryManagerVolunteer::decreaseCoolDown()
{
    timeLeft -= 1;
    if (timeLeft <= 0) {
        return true;
    }
    return false; 
}
// ATT: Im not sure I understood the question correctly, need to verify
bool InventoryManagerVolunteer::hasRequestsLeft() const
{
    return activeRequestId != NO_REQUEST;
}
bool InventoryManagerVolunteer::canTakeRequest(const SupplyRequest &request) const
{
    return !isBusy();
}
void InventoryManagerVolunteer::acceptRequest(const SupplyRequest &request)
{
    activeRequestId = request.getId();
    timeLeft = coolDown;
}
// ATT: I'm not sure of the format of the string, need to verify
string InventoryManagerVolunteer::toString() const
{
    std::ostringstream oss;
    oss << "Volunteer ID: " << getId() << "\n";
    oss << "Name: " << getName() << "\n";
    oss << "IsBusy: " << (isBusy() ? "True" : "False") << "\n";
    oss << "RequestID: " << (getActiveRequestId() != NO_REQUEST ? std::to_string(getActiveRequestId()) : "None") << "\n";
    oss << "TimeLeft: " << (isBusy() ? std::to_string(getTimeLeft()-1) : "None") << "\n";
    return oss.str();
}

// CourierVolunteer implementation
CourierVolunteer::CourierVolunteer(int id, const string &name, int maxDistance, int distancePerStep)
    : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

CourierVolunteer *CourierVolunteer::clone() const
{
    return new CourierVolunteer(*this);
}
int CourierVolunteer::getDistanceLeft() const
{
    return distanceLeft;
}
int CourierVolunteer::getMaxDistance() const
{
    return maxDistance;
}
int CourierVolunteer::getDistancePerStep() const
{
    return distancePerStep;
}
bool CourierVolunteer::decreaseDistanceLeft()
{
    distanceLeft -= distancePerStep;

    if (distanceLeft <= 0)
    {
        return true;
    }
    return false;
}
bool CourierVolunteer::hasRequestsLeft() const
{
    return activeRequestId != NO_REQUEST;
}
bool CourierVolunteer::canTakeRequest(const SupplyRequest &request) const
{
    if (!isBusy() && request.getDistance() <= maxDistance && request.getStatus() == RequestStatus::COLLECTING)
    {
        return true;
    }
    return false;
}
void CourierVolunteer::acceptRequest(const SupplyRequest &request)
{
    activeRequestId = request.getId();
    distanceLeft = request.getDistance();
    //std::cout << "CourierVolunteer ID " << getId() << "Accepted Request ID: " << activeRequestId << " with Distance Left: " << distanceLeft << std::endl;
}
// bool CourierVolunteer::coolDownFinished()
// {
//     return distanceLeft == 0;
// }

// void CourierVolunteer::step()
// {
//     distanceLeft = distanceLeft - distancePerStep;
//     std::cout << "(CourierVolunteer) Distance Left: " << distanceLeft << " " << activeRequestId << std::endl;
//     if (distanceLeft <= 0)
//     {
//         std::cout << "(CourierVolunteer) Request Completed " << activeRequestId << std::endl;
//         distanceLeft = 0;
//         completedRequestId = activeRequestId;
//         // activeRequestId = NO_REQUEST;
//     }
// }

void CourierVolunteer::step()
{
    if (activeRequestId != NO_REQUEST && distanceLeft >= 0)
    {
        distanceLeft -= distancePerStep;
        if (distanceLeft < 0)
        {
            distanceLeft = 0;
        }
        if (distanceLeft == 0)
        {
            std::cout << "(CourierVolunteer) Request Completed activeRequestId: " << activeRequestId << std::endl;
            completedRequestId = activeRequestId;
            std::cout << "(CourierVolunteer) Request Completed completedRequestId: " << completedRequestId << std::endl;
        }
    }
}

// ATT: I'm not sure of the format of the string, need to verify
string CourierVolunteer::toString() const
{
    //return "CourierVolunteer: " + getName() + " (ID: " + std::to_string(getId()) + ")";
    std::ostringstream oss;
    oss << "Courier ID: " << getId() << "\n";
    oss << "Name: " << getName() << "\n";
    oss << "IsBusy: " << (isBusy() ? "True" : "False") << "\n";
    oss << "RequestID: " << (getActiveRequestId() != NO_REQUEST ? std::to_string(getActiveRequestId()) : "None") << "\n";
    oss << "distanceLeft: " << distanceLeft << "\n";
    oss << "maxDistance: " << maxDistance << "\n";
    return oss.str();
    
}

void Volunteer::setNoActiveRequest()
{
    activeRequestId = NO_REQUEST;
}
