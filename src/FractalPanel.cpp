#include "FractalFrame.h"
#include "CURSOR_CROSS.h"

enum{
    ID_ChangeSettings = 1
};

///Constructor
FractalPanel::FractalPanel(FractalFrame* p, wxSize s)
    :wxPanel(p, wxID_ANY, wxDefaultPosition, s, wxBORDER_RAISED),
     parent(p){
    ///Initial settings
    SetCursor(CURSOR_CROSS);
    //popupMenu_ = new wxMenu;
    //popupMenu_->Append(ID_ChangeSettings, "Change settings");
}

BEGIN_EVENT_TABLE(FractalPanel, wxPanel)
    EVT_MOUSEWHEEL(FractalPanel::OnMouseCallback)
    EVT_SIZE      (FractalPanel::OnSizeCallback )
/*
    EVT_PAINT     (FractalFrame::FractalPanel::OnPaintEvent)
    EVT_RIGHT_UP  (FractalFrame::FractalPanel::OnRightUp   )
    EVT_SIZE      (FractalFrame::FractalPanel::OnSizeEvent )
    EVT_MENU      (ID_ChangeSettings, FractalFrame::FractalPanel::OnChangeSettings)
*/
END_EVENT_TABLE()
