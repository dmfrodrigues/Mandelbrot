#ifndef MYEVENTS_H_INCLUDED
#define MYEVENTS_H_INCLUDED

#include <wx/window.h>

class myMouseEvent{
public:
    ///CONSTRUCTOR
    myMouseEvent();
    myMouseEvent(wxMouseEvent event, bool flag = false);
    ///PUBLIC MEMBER FUNCTIONS
    bool IsInside(const wxWindow* obj) const;
    bool IsValid() const;
    bool& SetValid(bool flag);
    wxEventType GetEventType() const;
    float GetWheelRotation() const;
private:
    ///VARIABLES
    wxPoint position_;
    wxEventType eventType_;
    float wheelRotation_;
    bool isValid_;
};

class mySizeEvent{
public:
    ///CONSTRUCTOR
    mySizeEvent();
    mySizeEvent(wxSizeEvent event, bool flag = false);
    bool IsValid() const;
    bool SetValid(bool flag);
private:
    ///VARIABLES
    wxSize size_;
    bool isValid_;
};

#endif // MYEVENTS_H_INCLUDED
