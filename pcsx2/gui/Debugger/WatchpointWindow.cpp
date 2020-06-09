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

#include "PrecompiledHeader.h"
#include "WatchpointWindow.h"


wxBEGIN_EVENT_TABLE(WatchpointWindow, wxDialog)
	EVT_CHECKBOX(wxID_ANY, WatchpointWindow::onCheckChange)
	EVT_BUTTON(wxID_OK, WatchpointWindow::onButtonOk)
wxEND_EVENT_TABLE()

WatchpointWindow::WatchpointWindow( wxWindow* parent, DebugInterface* _cpu )
	: wxDialog(parent,wxID_ANY,L"Watchpoint"), cpu(_cpu)
{
	wxBoxSizer* topLevelSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* upperPart = new wxBoxSizer(wxHORIZONTAL);

	wxFlexGridSizer* leftSizer = new wxFlexGridSizer(2,2,7,7);
	wxBoxSizer* compModeSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* compValSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* bottomRightSizer = new wxBoxSizer(wxHORIZONTAL);

    wxArrayString registerNames;
    
    for(int iterCat = 0; iterCat < cpu->getRegisterCategoryCount(); iterCat++)
    for(int iterNum = 0; iterNum < cpu->getRegisterCount(iterCat); iterNum++){
        FastFormatUnicode registerName;
        registerName.Write("%s | %s",cpu->getRegisterCategoryName(iterCat),cpu->getRegisterName(iterCat,iterNum));
        registerNames.push_back(wxString(registerName.c_str()));
        registers.push_back(Register(iterCat,iterNum));
    }
	// register
	wxStaticText* registerText = new wxStaticText(this,wxID_ANY,L"Register");
	comboRegisters = 
    new wxComboBox(this,wxID_ANY,L"",wxDefaultPosition,wxDefaultSize,registerNames,wxCB_READONLY);
    
	leftSizer->Add(registerText,0,wxALIGN_CENTRE_VERTICAL|wxBOTTOM,1);
    leftSizer->Add(comboRegisters,1);

    // Left Padding
    compModeSizer->AddSpacer(7);
    // Align radio buttons with comboRegisters and textCompareValue
    compModeSizer->AddSpacer(registerText->GetSize().x);

    radioEQU = new wxRadioButton(this,wxID_ANY,L"==");
    compModeSizer->Add(radioEQU,0);

    radioGT = new wxRadioButton(this,wxID_ANY,L">");
    compModeSizer->Add(radioGT,0);

    radioGTE = new wxRadioButton(this,wxID_ANY,L"≥");
    compModeSizer->Add(radioGTE,0);

    radioLT = new wxRadioButton(this,wxID_ANY,L"<");
    compModeSizer->Add(radioLT,0);

    radioLTE = new wxRadioButton(this,wxID_ANY,L"≤");
    compModeSizer->Add(radioLTE,0);

    // Left Padding
    compValSizer->AddSpacer(7);

    wxStaticText* compText = new wxStaticText(this,wxID_ANY,L"Value");
    // Align text label with above label
    compText->SetMinSize(wxSize(registerText->GetSize().x,compText->GetSize().y));
    compValSizer->Add(compText,0,wxALIGN_CENTRE_VERTICAL|wxBOTTOM);

    compValSizer->AddSpacer(7);

    textCompareValue = new wxTextCtrl(this,wxID_ANY,L"");
    // Make textCtrl the same size as the above combobox for symmetry 
    textCompareValue->SetMinSize(wxSize(comboRegisters->GetSize().x,textCompareValue->GetSize().y));
    compValSizer->Add(textCompareValue,1);

    compValSizer->AddSpacer(2);

    checkOnChange = new wxCheckBox(this,wxID_ANY,L"On change");
    compValSizer->Add(checkOnChange,0,wxALIGN_CENTER_VERTICAL);

    bottomRightSizer->AddSpacer(registerText->GetSize().x + 8);    
    checkEnabled = new wxCheckBox(this,wxID_ANY,L"Enabled");
    bottomRightSizer->Add(checkEnabled,0,wxALIGN_CENTER_VERTICAL);

    bottomRightSizer->AddSpacer(8);

    buttonOk = new wxButton(this,wxID_OK,"Ok");
    bottomRightSizer->Add(buttonOk,0);

    bottomRightSizer->AddSpacer(4);

    buttonCancel = new wxButton(this,wxID_ANY,"Cancel");
    bottomRightSizer->Add(buttonCancel,0);

    bottomRightSizer->AddSpacer(8);

    
/*
	// size
	wxStaticText* sizeText = new wxStaticText(this,wxID_ANY,L"Size");
	editSize = new wxTextCtrl(this,wxID_ANY,L"");
	leftSizer->Add(sizeText,0,wxALIGN_CENTER_VERTICAL|wxBOTTOM,1);
	leftSizer->Add(editSize,1);

	// right part
	wxFlexGridSizer* rightSizer = new wxFlexGridSizer(3,2,7,7);

	radioMemory = new wxRadioButton(this,wxID_ANY,L"Memory",wxDefaultPosition,wxDefaultSize,wxRB_GROUP);
	radioExecute = new wxRadioButton(this,wxID_ANY,L"Execute");
	rightSizer->Add(radioMemory,1);
	rightSizer->Add(radioExecute,1,wxLEFT,8);

	checkRead = new wxCheckBox(this,wxID_ANY,L"Read");
	checkWrite = new wxCheckBox(this,wxID_ANY,L"Write");
	checkOnChange = new wxCheckBox(this,wxID_ANY,L"On change");
	rightSizer->Add(checkRead,1);
	rightSizer->Add(new wxStaticText(this,wxID_ANY,L""),1);
	rightSizer->Add(checkWrite,1);
	rightSizer->Add(checkOnChange,1,wxLEFT,8);
*/
	upperPart->Add(leftSizer,1,wxLEFT|wxTOP|wxRIGHT,8);
/*	upperPart->Add(rightSizer,1,wxLEFT|wxTOP|wxRIGHT,8);

	// bottom part
	wxBoxSizer* conditionSizer = new wxBoxSizer(wxHORIZONTAL);
	wxStaticText* conditionText = new wxStaticText(this,wxID_ANY,L"Condition");
	editCondition = new wxTextCtrl(this,wxID_ANY,L"");
	conditionSizer->Add(conditionText,0,wxALIGN_CENTER_VERTICAL|wxBOTTOM,1);
	conditionSizer->AddSpacer(7);
	conditionSizer->Add(editCondition,1,wxEXPAND);

	wxBoxSizer* bottomRowSizer = new wxBoxSizer(wxHORIZONTAL);
	checkEnabled = new wxCheckBox(this,wxID_ANY,L"Enabled");
	checkLog = new wxCheckBox(this,wxID_ANY,L"Log");
	buttonOk = new wxButton(this,wxID_OK,L"OK");
	buttonCancel = new wxButton(this,wxID_CANCEL,L"Cancel");
	bottomRowSizer->AddStretchSpacer();
	bottomRowSizer->Add(checkEnabled,0,wxALIGN_CENTER_VERTICAL|wxBOTTOM,1);
	bottomRowSizer->AddSpacer(8);
	bottomRowSizer->Add(checkLog,0,wxALIGN_CENTER_VERTICAL|wxBOTTOM,1);
	bottomRowSizer->AddSpacer(8);
	bottomRowSizer->Add(buttonOk,1);
	bottomRowSizer->AddSpacer(4);
	bottomRowSizer->Add(buttonCancel,1);

	// set tab order
	radioMemory->MoveAfterInTabOrder(editAddress);
	radioExecute->MoveAfterInTabOrder(radioMemory);
	editSize->MoveAfterInTabOrder(radioExecute);
	checkRead->MoveAfterInTabOrder(editSize);
	checkWrite->MoveAfterInTabOrder(checkRead);
	checkOnChange->MoveAfterInTabOrder(checkWrite);
	editCondition->MoveAfterInTabOrder(checkOnChange);
	checkEnabled->MoveAfterInTabOrder(editCondition);
	checkLog->MoveAfterInTabOrder(checkEnabled);
	buttonOk->MoveAfterInTabOrder(checkLog);
	buttonCancel->MoveAfterInTabOrder(buttonOk);
*/
	topLevelSizer->Add(upperPart,0);
    topLevelSizer->AddSpacer(7);
    topLevelSizer->Add(compModeSizer,0);
    topLevelSizer->AddSpacer(7);
    topLevelSizer->Add(compValSizer,0);
    topLevelSizer->AddSpacer(7);
    topLevelSizer->Add(bottomRightSizer,0,wxALIGN_RIGHT|wxALIGN_BOTTOM);
    topLevelSizer->AddSpacer(7);
	SetSizer(topLevelSizer);
	topLevelSizer->Fit(this);
/*
	buttonOk->SetDefault();

	// default values
	memory = true;
	onChange = false;
	read = write = true;
	enabled = log = true;
	address = -1;
	size = 1;
	condition[0] = 0;
	setDefaultValues();
    */
}

void WatchpointWindow::onCheckChange(wxCommandEvent& evt)
{
	onChange = checkOnChange->GetValue();
    textCompareValue->Enable(!onChange);
    radioEQU->Enable(!onChange);
    radioGT->Enable(!onChange);
    radioGTE->Enable(!onChange);
    radioLT->Enable(!onChange);
    radioLTE->Enable(!onChange);
}

bool WatchpointWindow::fetchDialogData()
{
	return true;
}

void WatchpointWindow::onButtonOk(wxCommandEvent& evt)
{
	if (fetchDialogData())
		evt.Skip();
}

void WatchpointWindow::addWatchpoint()
{
    WatchPointCond newWPC;

    newWPC.onChange = checkOnChange->GetValue();

    if(!newWPC.onChange){
        if (radioEQU->GetValue()){
            newWPC.compType = WPC_EQU;
        }
        else if (radioGT->GetValue()){
            newWPC.compType = WPC_GT;
        }
        else if (radioGTE->GetValue()){
            newWPC.compType = WPC_GTE;
        }
        else if (radioLT->GetValue()){
            newWPC.compType = WPC_LT;
        }
        else if (radioLTE->GetValue()){
            newWPC.compType = WPC_LTE;
        }
        u128 compVal;

        if(!convertU128(textCompareValue->GetValue(),compVal))
        {
            wchar_t errorMessage[512];
            swprintf(errorMessage,512,L"Invalid expression \"%s\".",textCompareValue->GetValue().wchar_str().data());
	    	wxMessageBox(errorMessage,L"Error",wxICON_ERROR);
            wxMutexGuiLeave();
	    	return;
        }

        newWPC.compVal = compVal;
    }
    
    newWPC.debug = cpu;
    
    int index = comboRegisters->GetCurrentSelection();

    newWPC.regCat = registers.at(index).cat;
    newWPC.regNum = registers.at(index).num;

    WatchPoint newWatchPoint;
    newWatchPoint.cond = newWPC;

    newWatchPoint.enabled = checkEnabled->GetValue();

    WatchPoint::watchPoints_.push_back(newWatchPoint);
}

bool WatchpointWindow::convertU128(const wxString src, u128& dest)
{
    if(src.size() > 16)
    { 
        wxCharBuffer compValueHI = src.data().AsCharBuf()[15];
        wxCharBuffer compValueLO(17);
        wxStrncat(compValueHI.data(),src.data(),16);

        if(!parseNumber(compValueLO,16,compValueLO.length(),dest.lo)) return false;
        return parseNumber(compValueHI,16,16,dest.hi);
    }
    else
    {
        dest.hi = 0;    // Make sure to clear the low bytes
        return parseNumber(src,16,src.length(),dest.lo);
    }
    return true;
}

/*
void BreakpointWindow::addBreakpoint()
{
	if (memory)
	{
		// add memcheck
		int cond = 0;
		if (read)
			cond |= MEMCHECK_READ;
		if (write)
			cond |= MEMCHECK_WRITE;
		if (onChange)
			cond |= MEMCHECK_WRITE_ONCHANGE;

		MemCheckResult result;
		if (log && enabled) result = MEMCHECK_BOTH;
		else if (log) result = MEMCHECK_LOG;
		else if (enabled) result = MEMCHECK_BREAK;
		else result = MEMCHECK_IGNORE;

		CBreakPoints::AddMemCheck(address, address + size, (MemCheckCondition)cond, result);
	} else {
		// add breakpoint
		CBreakPoints::AddBreakPoint(address,false);

		if (condition[0] != 0)
		{
			BreakPointCond cond;
			cond.debug = cpu;
			strcpy(cond.expressionString,condition);
			cond.expression = compiledCondition;
			CBreakPoints::ChangeBreakPointAddCond(address,cond);
		}

		if (!enabled)
		{
			CBreakPoints::ChangeBreakPoint(address,false);
		}
	}
}
*/