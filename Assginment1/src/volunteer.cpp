#include <string>
#include <vector>
#include <SupplyRequest.h>
#include <Volunteer.h>
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

// InventoryManagerVolunteer implementation
InventoryManagerVolunteer::InventoryManagerVolunteer(int id, const string &name, int coolDown)
    : Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

InventoryManagerVolunteer *InventoryManagerVolunteer::clone() const
{
    return new InventoryManagerVolunteer(*this);
}
void InventoryManagerVolunteer::step()
{
    if (activeRequestId != NO_REQUEST)
    {
        timeLeft--;
        // ATT: Do I need to check if timeLeft is less than 0?
        if (timeLeft == 0)
        {
            completedRequestId = activeRequestId;
            activeRequestId = NO_REQUEST;
        }
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
    if (timeLeft > 0)
    {
        timeLeft--;
        return false;
    }
    return true;
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
    return "InventoryManagerVolunteer: " + getName() + " (ID: " + std::to_string(getId()) + ")";
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
    if (distanceLeft > 0)
    {
        return false;
    }
    return true;
}
bool CourierVolunteer::hasRequestsLeft() const
{
    return activeRequestId != NO_REQUEST;
}
bool CourierVolunteer::canTakeRequest(const SupplyRequest &request) const
{
    return !isBusy() && request.getDistance() <= maxDistance; // request is yet to be implemented
}
void CourierVolunteer::acceptRequest(const SupplyRequest &request)
{
    activeRequestId = request.getId();
    distanceLeft = request.getDistance();
}
void CourierVolunteer::step()
{
    if (activeRequestId != NO_REQUEST)
    {
        distanceLeft -= distancePerStep;
        if (distanceLeft <= 0)
        {
            completedRequestId = activeRequestId;
            activeRequestId = NO_REQUEST;
        }
    }
}

// ATT: I'm not sure of the format of the string, need to verify
string CourierVolunteer::toString() const
{
    return "CourierVolunteer: " + getName() + " (ID: " + std::to_string(getId()) + ")";
}