#include <wx/bitmap.h>
wxCursor CreateCURSOR_CROSS(){
    wxInitAllImageHandlers();
    wxImage CROSS_image("CROSS_CURSOR.png");
    CROSS_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_X, 15);
    CROSS_image.SetOption(wxIMAGE_OPTION_CUR_HOTSPOT_Y, 15);
    wxCursor CROSS_cursor(CROSS_image);
    return CROSS_cursor;
}
const wxCursor CURSOR_CROSS = CreateCURSOR_CROSS();
