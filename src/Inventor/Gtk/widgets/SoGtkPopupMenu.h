#ifndef SOGTK_POPUPMENU_H
#define SOGTK_POPUPMENU_H

#include <Inventor/Gtk/widgets/SoAnyPopupMenu.h>

// *************************************************************************

class SoGtkPopupMenu : public SoAnyPopupMenu {
public:
  SoGtkPopupMenu(void);
  ~SoGtkPopupMenu(void);

  virtual int NewMenu( char * name, int menuid = -1 );
  virtual int GetMenu( char * name );
  virtual void SetMenuTitle( int id, char * title );
  virtual char * GetMenuTitle( int id );

  virtual int NewMenuItem( char * name, int itemid = -1 );
  virtual int GetMenuItem( char * name );
  virtual void SetMenuItemTitle( int itemid, char * title );
  virtual char * GetMenuItemTitle( int itemid );
  virtual void SetMenuItemEnabled( int itemid, SbBool enabled );
  virtual SbBool GetMenuItemEnabled( int itemid );
  virtual void SetMenuItemMarked( int itemid, SbBool marked );
  virtual SbBool GetMenuItemMarked( int itemid );

  virtual void AddMenu( int menuid, int submenuid, int pos = -1 );
  virtual void AddMenuItem( int menuid, int itemid, int pos = -1 );
  virtual void AddSeparator( int menuid, int pos = -1 );
  virtual void RemoveMenu( int menuid );
  virtual void RemoveMenuItem( int itemid );

  virtual void PopUp( GtkWidget * inside, int x, int y );

}; // class SoGtkPopupMenu

// *************************************************************************

#endif // ! SOGTK_POPUPMENU_H
