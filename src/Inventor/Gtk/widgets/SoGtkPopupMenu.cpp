
#include <Inventor/Gtk/SoGtkBasic.h>
#include <Inventor/Gtk/widgets/SoGtkPopupMenu.h>

// *************************************************************************

SoGtkPopupMenu::SoGtkPopupMenu(
  void )
{
} // SoGtkPopupMenu()

SoGtkPopupMenu::~SoGtkPopupMenu(
  void )
{
} // ~SoGtkPopupMenu()

// *************************************************************************

int
SoGtkPopupMenu::NewMenu( char * name, int menuid )
{
  SOGTK_STUB();
  return -1;
}

int
SoGtkPopupMenu::GetMenu( char * name )
{
  SOGTK_STUB();
  return -1;
}

void
SoGtkPopupMenu::SetMenuTitle( int id, char * title )
{
  SOGTK_STUB();
}

char *
SoGtkPopupMenu::GetMenuTitle( int id )
{
  SOGTK_STUB();
  return NULL;
}

int
SoGtkPopupMenu::NewMenuItem( char * name, int itemid )
{
  SOGTK_STUB();
  return -1;
}

int
SoGtkPopupMenu::GetMenuItem( char * name )
{
  SOGTK_STUB();
  return -1;
}

void
SoGtkPopupMenu::SetMenuItemTitle( int itemid, char * title )
{
  SOGTK_STUB();
}

char *
SoGtkPopupMenu::GetMenuItemTitle( int itemid )
{
  SOGTK_STUB();
  return NULL;
}

void
SoGtkPopupMenu::SetMenuItemEnabled( int itemid, SbBool enabled )
{
  SOGTK_STUB();
}

SbBool
SoGtkPopupMenu::GetMenuItemEnabled( int itemid )
{
  SOGTK_STUB();
  return FALSE;
}

void
SoGtkPopupMenu::SetMenuItemMarked( int itemid, SbBool marked )
{
  SOGTK_STUB();
}

SbBool
SoGtkPopupMenu::GetMenuItemMarked( int itemid )
{
  SOGTK_STUB();
  return FALSE;
}

void
SoGtkPopupMenu::AddMenu( int menuid, int submenuid, int pos )
{
  SOGTK_STUB();
}

void
SoGtkPopupMenu::AddMenuItem( int menuid, int itemid, int pos )
{
  SOGTK_STUB();
}

void
SoGtkPopupMenu::AddSeparator( int menuid, int pos )
{
  SOGTK_STUB();
}

void
SoGtkPopupMenu::RemoveMenu( int menuid )
{
  SOGTK_STUB();
}

void
SoGtkPopupMenu::RemoveMenuItem( int itemid )
{
  SOGTK_STUB();
}

void
SoGtkPopupMenu::PopUp( GtkWidget * inside, int x, int y )
{
  SOGTK_STUB();
}

// *************************************************************************
