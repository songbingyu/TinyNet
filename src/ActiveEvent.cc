/*
 *  CopyRight  2014 , bingyu.song   All Right Reserved
    I believe  Spring brother
 */
#include "ActiveEvent.h"
#include "Event.h"
#include "EventLoop.h"

void ActiveFdEvent::addList(EventIo* ev)
{
    *ev->getNext() = head_;
    head_ = ev;
}

void ActiveFdEvent::delList(EventIo* ev)
{
    EventList** head = (EventList**)&head_;
    while (*head) {

        if (expect_true(*head == ev)) {
            *head = *ev->getNext();
            break;
        }

        head =(*head)->getNext();
    }
}

void ActiveFdEvent::killFd(EventLoop* loop)
{
    EventIo* ev = NULL;
    while ((ev = head_)) {
        ev->stop();
        loop->addPendingEvent((IEvent*)ev, EV_ERROR | EV_WRITE | EV_READ);
    }

}

void ActiveFdEvent::fdEvent(EventLoop* loop, int revents)
{
    EventList* head = head_;
    for ( ; head != NULL; head = *head->getNext()) {
        loop->addPendingEvent((IEvent*)head , revents);
    }

}

void ActiveSignalEvent::addList(EventList* el)
{
    *el->getNext() = head_;
    head_ = el;
}

void ActiveSignalEvent::delList(EventList* el)
{
    EventList** head = (EventList**)&head_;
    while (*head) {
        if (expect_true(*head == el)) {
            *head = *el->getNext();
            break;
        }

        head = (*head)->getNext();
    }
}

void ActiveSignalEvent::addFeedEvent()
{
    EventList* head = head_;
    for ( ; head != NULL; head = *head->getNext()) {
        loop_->addPendingEvent((IEvent*)head , EV_SIGNAL);
    }
}



