#include "MyApp.h"

bool MyApp::OnInit(){   
        
        // make sure to call this first
        wxInitAllImageHandlers();
        
        wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
        frame = new wxFrame(NULL, wxID_ANY, wxT("TransitoHCP"), wxPoint(50,50), wxSize(800,600));
        frame -> SetMinSize(wxSize(600,400)); 
        // then simply create like this
        drawPane = new wxImagePanel( frame, wxT("./figs/Unal-CU-Bogota.jpg"), wxBITMAP_TYPE_JPEG);
        
       
        sizer->Add(drawPane, 1, wxEXPAND);

        
        frame->SetSizer(sizer);
        
        frame->Show();
        return true;
    } 