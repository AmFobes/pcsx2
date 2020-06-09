/*  PCSX2 - PS2 Emulator for PCs
 *  Copyright (C) 2002-2014  PCSX2 Dev Team
 *
 *  PCSX2 is free software: you can redistribute it and/or modify it under the terms
 *  of the GNU Lesser General Public License as published by the Free Software Found-
 *  ation, either version 3 of the License, or (at your option) any later version.
 *
 *  PCSX2 is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 *  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 *  PURPOSE.  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along with PCSX2.
 *  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <wx/wx.h>
#include "DebugTools/DebugInterface.h"
#include "DebugTools/Watchpoints.h"

struct Register{
    u32 cat;
    u32 num;
    Register(u32 _cat, u32 _num){
        cat = _cat;
        num = _num;
    }
};

class WatchpointWindow : public wxDialog
{
public:
	WatchpointWindow( wxWindow* parent, DebugInterface* _cpu );
	void loadFromWatchpoint(WatchPoint& watchpoint);
	void initBreakpoint(u32 _address);
	void addWatchpoint();

	wxDECLARE_EVENT_TABLE();
protected:
	void onCheckChange(wxCommandEvent& evt);
	void onButtonOk(wxCommandEvent& evt);
private:
	void setDefaultValues();
	bool fetchDialogData();
    bool convertU128(const wxString src, u128& dest);
	
    DebugInterface* cpu;
	std::vector<Register> registers;
    
	wxCheckBox* checkOnChange;
	wxComboBox* comboRegisters;
	wxCheckBox* checkEnabled;
    wxRadioButton* radioEQU;
    wxRadioButton* radioGT;
    wxRadioButton* radioGTE;
    wxRadioButton* radioLT;
    wxRadioButton* radioLTE;
    wxTextCtrl* textCompareValue;
	wxButton* buttonOk;
	wxButton* buttonCancel;

	bool onChange;
    u32 registerCategory;
    u32 registerNumber;
    bool enabled;

};
