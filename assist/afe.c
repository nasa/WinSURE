/* ------------------------------------------------------------------------

   Assist Windows Interface 

   Author: Ricky W. Butler                   NASA Langley Research Center
           R.W.Butler@nasa.larc.gov

   This is the callback routine that creates the interface window.
   It calls assistbody in assist.c to do the real work.

   Version 1.4            3/30/2006
-------------------------------------------------------------------------- */
#define WINVER 0x0500
#define MX_BF_LEN 500000
#ifdef MSDOS
#include <windows.h>
#include <string.h>
#include <commdlg.h>
#include <stdio.h>
#include "afe.h"


#include <setjmp.h>
jmp_buf ProcessExit;

/* the following definitions are defined elsewhere, so change in 2 places */
typedef unsigned char Boolean;
extern Boolean xref_map;
extern short max_input_line_length;  /* maximum number of chars/line input */
extern set_ONEDEATH_default(int);
extern set_COMMENTS_default(int);
#define LINE_MAXNCH 256
#define LINE_MINCH 40

typedef char buftype[255];
buftype win_buf[MX_BF_LEN]; 

buftype str;              /* holds output strings */
buftype outstr;
FILE *outstuff; 

static int numlines = 10;     /* num of lines written to window */
static int inlastcmd = 0;       /* is zero if last command was Input_File */


/* holds status of check box */
int cb_map_status=BST_UNCHECKED;
int cb_one_death=BST_CHECKED;
int cb_state_comments=BST_CHECKED;



LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;   /* forward decl */
BOOL CALLBACK DialogFunc(HWND, UINT, WPARAM, LPARAM);     // for dialog box


HFONT hnewf;

HINSTANCE hInst;        // For Dialog Box

int WINAPI WinMain (HINSTANCE hThisInst, HINSTANCE hPrevInst,
                    LPSTR lpszArgs, int nWinMode) // iCmdShow)
{
  static TCHAR szAppName[] = TEXT ("MyWin") ;
  HWND         hwnd ;
  MSG          msg ;
  WNDCLASSEX   wcl ;
  HACCEL       hAccel;


  /* Define a window class. */
  wcl.cbSize = sizeof(WNDCLASSEX); 

  wcl.hInstance     = hThisInst ;               /* handle to this instance */
  wcl.lpszClassName = szAppName ;               /* window class name */
  wcl.lpfnWndProc   = WndProc ;                 /* window function */
  wcl.style         = CS_HREDRAW | CS_VREDRAW ;

  wcl.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ; /* stand icon */
  wcl.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);   /* small icon */
  wcl.hCursor       = LoadCursor (NULL, IDC_ARROW) ;     /* cursor style */

  wcl.lpszMenuName = "InputFile";                /* main menu */
  wcl.cbClsExtra    = 0 ;
  wcl.cbWndExtra    = 0 ;

  /* Make the window white. */
  wcl.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH); 

     
  if (!RegisterClassEx (&wcl))
  {
       MessageBox (NULL, TEXT ("Program requires Windows NT!"), 
                   szAppName, MB_ICONERROR) ;
       return 0 ;
  }
     
  hwnd = CreateWindow (szAppName, TEXT ("WinASSIST 1.4"),
                          WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
       CW_USEDEFAULT,                  /* X coordinate - let Windows decide */
       CW_USEDEFAULT,                  /* Y coordinate - let Windows decide */
       CW_USEDEFAULT,                  /* width - let Windows decide */
       CW_USEDEFAULT,                  /* height - let Windows decide */
       HWND_DESKTOP,                   /* no parent window */
       NULL,                           /* no override of class menu */
       hThisInst,                      /* handle of this instance of program */
       NULL                            /* no additional arguments */
     );

  hInst = hThisInst;      // save the current instance handle for dialogfunc 


  max_input_line_length=120;       /*  Now controlled by Windows options */



  /* Load accelerators, needed for menu */
  hAccel = LoadAccelerators(hThisInst, "InputFile");

  ShowWindow (hwnd, nWinMode); 
   UpdateWindow (hwnd) ;
  
  while (GetMessage (&msg, NULL, 0, 0))
  {
       TranslateMessage (&msg) ;       /* translate keyboard messages */
       DispatchMessage (&msg) ;        /* return control to Windows 98 */
  }
  return msg.wParam ;
}


int out_to_outbuf(int ix_init)
{int ix;
          ix = ix_init;
          while ( !feof(outstuff) ){ 
             fgets(outstr,250,outstuff);
             ix = ix + 1;
             if (ix >= MX_BF_LEN){
	       strncpy(win_buf[ix-1],
                "*** Listing exceeded capacity, see file.   ",43);
               break; }
             strncpy(win_buf[ix],outstr,250);
	  };
          return ix;
}

/* =========================================================================
   This function is called by Windows and provided with messages from the 
   message queue.
========================================================================= */
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  static int  cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth ;
  HDC         hdc ;
  int         i, x, y, iVertPos, iHorzPos, iPaintBeg, iPaintEnd ;
  PAINTSTRUCT ps ;
  SCROLLINFO  si ;
  TCHAR       szBuffer[10] ;
  TEXTMETRIC  tm ;

  int response, ix;

  //  FILE *outstuff; 

  OPENFILENAME fname;             /* model file opened via dialog box */
  char filename[130];             /* file name */
  char fn_new[256];               /* file name */
  static char fn[256];            /* full path name */
  char filefilter[] = "Assist\0*.ast\0All\0*.*\0\0";

  int PE_Status;
  
  switch (message)
  {
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  case WM_CREATE:
       hdc = GetDC (hwnd) ;
       
       GetTextMetrics (hdc, &tm) ;
       cxChar = tm.tmAveCharWidth ;
       cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2 ;
       cyChar = tm.tmHeight + tm.tmExternalLeading ;
       
       ReleaseDC (hwnd, hdc) ;

            // Save the width of the three columns
       
       iMaxWidth = 40 * cxChar + 22 * cxCaps ;
       return 0 ;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  case WM_COMMAND:              // called when Menu is accessed
      switch(LOWORD(wParam)) {
        case IDM_INPUTFILE:
        case IDM_RERUN:
          if (LOWORD(wParam) == IDM_INPUTFILE){
          /* first, get the name of the file to be analyzed. */

          /* initialize the OPENFILENAME struct */
          fname.lStructSize = sizeof(OPENFILENAME);
          fname.hwndOwner = hwnd;
          fname.lpstrFilter = filefilter; 
          fname.nFilterIndex = 1;
          fname.lpstrFile = fn;
          fname.nMaxFile = sizeof(fn);
          fname.lpstrFileTitle = filename;
          fname.nMaxFileTitle = sizeof(filename)-1;
          fname.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
          fname.lpstrCustomFilter = NULL;
          fname.lpstrInitialDir = NULL;
          fname.lpstrTitle = NULL;
          fname.lpstrDefExt = NULL;
          fname.lCustData = 0;

          if(!GetOpenFileName(&fname)) /* get the file name */
            break;
         
          strncpy(win_buf[0]," ",2);
	  sprintf(outstr, "Model File = %s ", filename);
          strncpy(win_buf[1],outstr,250);
          strncpy(win_buf[2]," ",2);
          }
          InvalidateRect(hwnd, NULL, 1);          /* cause WM_PAINT */


	  /* ------- Process Options --------- */
          if(cb_map_status == BST_CHECKED){
            //sprintf(outstr, "MAP Model File = %s ", filename);
            //strncpy(win_buf[1],outstr,250);
	    xref_map = TRUE;}
          else {
	    //sprintf(outstr, "NO MAP Model File = %s ", filename);
            //strncpy(win_buf[1],outstr,250);
	    xref_map = FALSE;}
           if(cb_one_death == BST_CHECKED)
	      set_ONEDEATH_default(1);
           else
              set_ONEDEATH_default(0);
           if(cb_state_comments == BST_CHECKED)
	      set_COMMENTS_default(1);
           else
              set_COMMENTS_default(0);

	   /* --------- call ASSIST ----------- */
          PE_Status = setjmp(ProcessExit);
	  if ( PE_Status != 0) {goto PE_label;}

          assistbody(fn);  /*  Call ASSIST model parsing and generation */
       	  astgen_free();   // attempt to get rid of memory leak

       PE_label:           // jump here if there is an error in assistbody

          fclose(stderr);
	  /* ----- display contents of STDERR.DAT in Window ----- */

          outstuff = fopen("STDERR.DAT", "r"); 

          ix = out_to_outbuf(2);
        
          fclose(outstuff);   // RWB05 changed close to fclose

	  /* ----- display contents of OUTPUT.DAT in Window ----- */
         
          outstuff = fopen("OUTPUT.DAT", "r"); 
         
          ix = out_to_outbuf(ix);

	  numlines = ix;
          si.nMax   = numlines - 1 ;         /* adjust size of scroll range */
          SetScrollRange (hwnd, SB_VERT, 0,numlines, TRUE);

          fclose(outstuff); 

          inlastcmd = 0;       /* is zero if last command was Input_File */
          InvalidateRect(hwnd, NULL, 1);   /* repaint the screen */

          break;

        case IDM_LISTMOD:
        case IDM_LISTING:
	  /* ----- display contents of filename in Window ----- */

          strcpy(fn_new,fn);
          remove_extent(".ast",fn_new);
          if (LOWORD(wParam) == IDM_LISTMOD)
             strcat(fn_new,".mod");
          else
            strcat(fn_new,".alg");

          outstuff = fopen(fn_new, "r");

          if (outstuff==((FILE *) NULL)) {
	    strncpy(win_buf[0],"  ",2);
            strncpy(win_buf[1],"*** FILE NOT FOUND ***",25);
	    ix = 2;}
          else {
            strncpy(win_buf[0]," ",2);
            if (LOWORD(wParam) == IDM_LISTMOD) 
	       sprintf(outstr, "Generated Model File = %s ", fn_new);
            else
	       sprintf(outstr, "   ");
            strncpy(win_buf[1],outstr,250);
            strncpy(win_buf[2]," ",2);

            ix = 2;
            ix = out_to_outbuf(ix);
 
	  };          
          fclose(outstuff);   

          inlastcmd = 1;       /* is zero if last command was Input_File */
	  numlines = ix;
	  si.nMax   = numlines - 1 ;       /* adjust size of scroll range */
          SetScrollRange (hwnd, SB_VERT, 0,numlines, TRUE);

          InvalidateRect(hwnd, NULL, 1);   /* repaint the screen */

          break;





        case IDM_DIALOG1: 
	  DialogBox(hInst, "MyDB", hwnd, (DLGPROC) DialogFunc);
          break;

        case IDM_EXIT:
          response = MessageBox(hwnd, "Quit the Program?",
                                "Exit", MB_YESNO);
          if(response == IDYES) PostQuitMessage(0);
          break;
        case IDM_HELP:
          sprintf(str,"(0) Use favorite text editor to create ASSIST Input File.\n");
          strcat(str,"(1) Use 'Input_File' to Select ASSIST Input File.\n");
          strcat(str,"(2) Use 'Listing' to examine syntax errors.\n");
          strcat(str,"(3) Use 'List_Mod' to see generated SemiMarkov Model.\n");
          strcat(str,"\n Use Options prior to step (1) to change some ASSIST parameters.\n");
          strcat(str,"\n Use Rerun after changing model in text editor.\n");
          MessageBox(hwnd, str,
                     "Help", MB_OK);
          break;
      }
      //break;
      return 0;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  case WM_SIZE:
     cxClient = LOWORD (lParam) ;
     cyClient = HIWORD (lParam) ;

          // Set vertical scroll bar range and page size

     si.cbSize = sizeof (si) ;
     si.fMask  = SIF_RANGE | SIF_PAGE ;
     si.nMin   = 0 ;
     si.nMax   = numlines - 1 ;
     si.nPage  = cyClient / cyChar ;
     SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;

          // Set horizontal scroll bar range and page size

     si.cbSize = sizeof (si) ;
     si.fMask  = SIF_RANGE | SIF_PAGE ;
     si.nMin   = 0 ;
     si.nMax   = 2 + iMaxWidth / cxChar ;
     si.nPage  = cxClient / cxChar ;
     SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
     return 0 ;
   
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
    case WM_VSCROLL:
    // Get all the vertial scroll bar information

    si.cbSize = sizeof (si) ;
    si.fMask  = SIF_ALL ;
    GetScrollInfo (hwnd, SB_VERT, &si) ;

         // Save the position for comparison later on

    iVertPos = si.nPos ;

    switch (LOWORD (wParam))
    {
    case SB_TOP:
         si.nPos = si.nMin ;
         break ;
         
    case SB_BOTTOM:
         si.nPos = si.nMax ;
         break ;
         
    case SB_LINEUP:
         si.nPos -= 1 ;
         break ;
         
    case SB_LINEDOWN:
         si.nPos += 1 ;
         break ;
         
    case SB_PAGEUP:
         si.nPos -= si.nPage ;
         break ;
         
    case SB_PAGEDOWN:
         si.nPos += si.nPage ;
         break ;
         
    case SB_THUMBTRACK:
         si.nPos = si.nTrackPos ;
         break ;
         
    default:
         break ;         
    }
         // Set the position and then retrieve it.  Due to adjustments
         //   by Windows it may not be the same as the value set.

    si.fMask = SIF_POS ;
    SetScrollInfo (hwnd, SB_VERT, &si, TRUE) ;
    GetScrollInfo (hwnd, SB_VERT, &si) ;

         // If the position has changed, scroll the window and update it

    if (si.nPos != iVertPos)
    {                    
         ScrollWindow (hwnd, 0, cyChar * (iVertPos - si.nPos), 
                             NULL, NULL) ;
         UpdateWindow (hwnd) ;
    }
    return 0 ;
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
  case WM_HSCROLL:
      // Get all the vertical scroll bar information

      si.cbSize = sizeof (si) ;
      si.fMask  = SIF_ALL ;

           // Save the position for comparison later on

      GetScrollInfo (hwnd, SB_HORZ, &si) ;
      iHorzPos = si.nPos ;

      switch (LOWORD (wParam))
      {
      case SB_LINELEFT:
           si.nPos -= 1 ;
           break ;
           
      case SB_LINERIGHT:
           si.nPos += 1 ;
           break ;
           
      case SB_PAGELEFT:
           si.nPos -= si.nPage ;
           break ;
           
      case SB_PAGERIGHT:
           si.nPos += si.nPage ;
           break ;
           
      case SB_THUMBPOSITION:
           si.nPos = si.nTrackPos ;
           break ;
           
      default :
           break ;
      }
           // Set the position and then retrieve it.  Due to adjustments
           //   by Windows it may not be the same as the value set.

      si.fMask = SIF_POS ;
      SetScrollInfo (hwnd, SB_HORZ, &si, TRUE) ;
      GetScrollInfo (hwnd, SB_HORZ, &si) ;
      
           // If the position has changed, scroll the window 

      if (si.nPos != iHorzPos)
      {
           ScrollWindow (hwnd, cxChar * (iHorzPos - si.nPos), 0, 
                         NULL, NULL) ;
      }
      return 0 ;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  case WM_PAINT :
     hdc = BeginPaint (hwnd, &ps) ;

          // Get vertical scroll bar position

     si.cbSize = sizeof (si) ;
     si.fMask  = SIF_POS ;
     GetScrollInfo (hwnd, SB_VERT, &si) ;
     iVertPos = si.nPos ;

          // Get horizontal scroll bar position

     GetScrollInfo (hwnd, SB_HORZ, &si) ;
     iHorzPos = si.nPos ;

          // Find painting limits

     iPaintBeg = max (0, iVertPos + ps.rcPaint.top / cyChar) ;
     iPaintEnd = min (numlines-1,
                      iVertPos + ps.rcPaint.bottom / cyChar) ;

     //sprintf(str, "iPaintBeg, iPaintEnd = %i,%i",iPaintBeg,iPaintEnd);
     //TextOut(hdc,cxChar*(1-iHorzPos),cyChar*(2-iVertPos),str,strlen(str)); 

     if (inlastcmd == 0) {
        if (iPaintBeg < 3)
          hnewf = (HFONT) GetStockObject(SYSTEM_FONT);
        else
          hnewf = (HFONT) GetStockObject(ANSI_VAR_FONT);}
     else
        hnewf = (HFONT) GetStockObject(ANSI_FIXED_FONT);

     SelectObject(hdc,hnewf);

     for (i = iPaintBeg ; i <= iPaintEnd ; i++)
     {
          x = cxChar * (1 - iHorzPos) ;
          y = cyChar * (i - iVertPos) ;
 
          if ((i == 3) && (inlastcmd == 0))  {
          	   hnewf = (HFONT) GetStockObject(ANSI_VAR_FONT);
          	   SelectObject(hdc,hnewf);}
         
          TextOut (hdc, x, y,
                   win_buf[i],
                   //"ABCDEF",6);
                   lstrlen (win_buf[i])-1) ;
   
          // SetTextAlign (hdc, TA_RIGHT | TA_TOP) ;
          // SetTextAlign (hdc, TA_LEFT | TA_TOP) ;
     }

     EndPaint (hwnd, &ps) ;
     return 0 ;
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++  
    case WM_DESTROY :
      PostQuitMessage (0) ;
      return 0 ;
  default:
      /* Let Windows 98 process any messages not specified in
         the preceding switch statement. */
      return DefWindowProc(hwnd, message, wParam, lParam);
  }
  return DefWindowProc (hwnd, message, wParam, lParam) ;
}


/* A simple dialog function. */
BOOL CALLBACK DialogFunc(HWND hdwnd, UINT message,
                         WPARAM wParam, LPARAM lParam)
{
  long i;
  char str[80];

  switch(message) {
    case WM_COMMAND:
      switch(LOWORD(wParam)) {
        case IDOK: /* edit box OK button selected */
          /* display contents of the edit box */
          GetDlgItemText(hdwnd, ID_EB1, str, 80);
          max_input_line_length = (int) strtol(str,(char **) NULL,10);
          if (max_input_line_length>LINE_MAXNCH)
             max_input_line_length=LINE_MAXNCH;
           else if (max_input_line_length<LINE_MINCH)
              max_input_line_length=LINE_MINCH;
          
	  // sprintf(str, "max_input_line_length = %i ",max_input_line_length);
          // MessageBox(hdwnd, str, "Edit Box Contains", MB_OK);

          cb_map_status = SendDlgItemMessage(hdwnd, ID_CB1,
                                         BM_GETCHECK, 0, 0); 

          cb_one_death = SendDlgItemMessage(hdwnd, ID_CB2,
                                         BM_GETCHECK, 0, 0); 

          cb_state_comments = SendDlgItemMessage(hdwnd, ID_CB3,
                                         BM_GETCHECK, 0, 0); 
          EndDialog(hdwnd, 0);


          return 1;
        case IDCANCEL:
          // DestroyWindow(hdwnd);
          EndDialog(hdwnd,0);
          InvalidateRect(hdwnd, NULL, 1);          /* cause WM_PAINT */
          return 1;

        case ID_LB1: /* process a list box LBN_DBLCLK */
          // see  if user made a selection
          if(HIWORD(wParam)==LBN_DBLCLK) {
            i = SendDlgItemMessage(hdwnd, ID_LB1,
                    LB_GETCURSEL, 0, 0);  // get index
            sprintf(str, "Index in list is: %d", i);
            MessageBox(hdwnd, str, "Selection Made", MB_OK);
          }
          return 1;
      }
      break;
    case WM_INITDIALOG: // initialize check box
      /* The dialog box has just been displayed.  Set
	 the check boxes appropriately. */
      SendDlgItemMessage(hdwnd, ID_CB1, BM_SETCHECK, cb_map_status, 0);
      SendDlgItemMessage(hdwnd, ID_CB2, BM_SETCHECK, cb_one_death, 1);
      SendDlgItemMessage(hdwnd, ID_CB3, BM_SETCHECK, cb_state_comments, 1);

      sprintf(str,"%i",max_input_line_length);
      SetDlgItemText(hdwnd, ID_EB1,str);


      return 1;
  }
  return 0;
}
#else
#endif
