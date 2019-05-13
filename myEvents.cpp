#include "myEvents.h"

///MYMOUSEEVENT
myMouseEvent::myMouseEvent(){}
myMouseEvent::myMouseEvent(wxMouseEvent event, bool flag)
                          :position_(event.GetPosition()),
                           eventType_(event.GetEventType()),
                           isValid_(flag){
    if(event.GetWheelDelta() != 0) wheelRotation_ = (float)event.GetWheelRotation()/(float)event.GetWheelDelta();
    else                           wheelRotation_ = 0.0L;
}
bool myMouseEvent::IsInside(const wxWindow* obj) const{
    wxSize szObj = obj->GetSize();
    return(0 <= position_.x && position_.x <= szObj.x &&
           0 <= position_.y && position_.y <= szObj.y);
}
bool  myMouseEvent::IsValid() const {return this->isValid_;}
bool& myMouseEvent::SetValid(bool flag){return(this->isValid_ = flag);}
wxEventType myMouseEvent::GetEventType() const {return this->eventType_;}
float myMouseEvent::GetWheelRotation() const {return this->wheelRotation_;};
///MYSIZEEVENT
mySizeEvent::mySizeEvent(){}
mySizeEvent::mySizeEvent(wxSizeEvent event , bool flag)
                        :size_(event.GetSize()),    isValid_(flag){}
bool mySizeEvent::IsValid() const {return this->isValid_;}
bool mySizeEvent::SetValid(bool flag){return(this->isValid_ = flag);}
