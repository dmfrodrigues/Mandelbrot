#ifndef FRACTALPANEL_H_INCLUDED
#define FRACTALPANEL_H_INCLUDED

class FractalFrame;

class FractalPanel: public wxPanel {
friend class FractalFrame;
public:
    FractalPanel(FractalFrame* p, wxSize s);
private:
    FractalFrame    *parent;

    wxMouseEvent mouseevt; bool is_mouseevt_handled = true;
    void OnMouseCallback(wxMouseEvent& evt){ mouseevt = evt; is_mouseevt_handled = false; };
    bool is_sizeevt_handled = true;
    void OnSizeCallback(wxSizeEvent& evt){ is_sizeevt_handled = false; };

    wxDECLARE_EVENT_TABLE();
};

#endif
