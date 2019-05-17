#include <wx/bitmap.h>
static unsigned char uCROSS_bits[] = {
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
      0, 128,   0,   0,
    255, 255, 255, 255,
      0, 128,   0,   0,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    255, 191, 254, 255,
    };
static unsigned char uCROSS_mask[] = {
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
      0,   0,   0, 128,
      0,   0,   0, 128,
      0,   0,   0, 128,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255,  63, 254, 255,
    255, 255, 255, 255,
    };
static char *CROSS_bits = reinterpret_cast<char*>(uCROSS_bits);
static char *CROSS_mask = reinterpret_cast<char*>(uCROSS_mask);
wxCursor CreateCURSOR_CROSS(){
    #ifdef __WXMSW__
        wxBitmap CROSS_bitmap(CROSS_bits, 32, 32);
        wxBitmap CROSS_mask_bitmap(CROSS_mask, 32, 32);
        CROSS_bitmap.SetMask(new wxMask(CROSS_mask_bitmap));
        wxImage CROSS_image = CROSS_bitmap.ConvertToImage();
        CROSS_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 15);
        CROSS_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 15);
        wxCursor CROSS_cursor = wxCursor(CROSS_image);
    #elif defined(__WXGTK__) or defined(__WXMOTIF__)
        wxCursor CROSS_cursor = wxCursor(CROSS_bits, 32, 32, 6, 14,
                                        CROSS_mask, wxWHITE, wxBLACK);
    #endif
    return CROSS_cursor;
}
const wxCursor CURSOR_CROSS = CreateCURSOR_CROSS();


