;; /**************************************************************************\
;;  * Copyright (c) Kongsberg Oil & Gas Technologies AS
;;  * All rights reserved.
;;  * 
;;  * Redistribution and use in source and binary forms, with or without
;;  * modification, are permitted provided that the following conditions are
;;  * met:
;;  * 
;;  * Redistributions of source code must retain the above copyright notice,
;;  * this list of conditions and the following disclaimer.
;;  * 
;;  * Redistributions in binary form must reproduce the above copyright
;;  * notice, this list of conditions and the following disclaimer in the
;;  * documentation and/or other materials provided with the distribution.
;;  * 
;;  * Neither the name of the copyright holder nor the names of its
;;  * contributors may be used to endorse or promote products derived from
;;  * this software without specific prior written permission.
;;  * 
;;  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
;;  * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
;;  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
;;  * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
;;  * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
;;  * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
;;  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
;;  * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
;;  * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
;;  * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;;  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;; \**************************************************************************/

(def-wrapped GtkWidget
  (scm-wrap "")
  (scm-unwrap ""))

(def-wrapped GdkEvent
  (scm-wrap "")
  (scm-unwrap ""))

(using Coin)

;; -- SoGtk ----------------------------------------------------------------

(def-cppclass SoGtk ()
  (header "<Inventor/Gtk/SoGtk.h>")
  (static #t))

(def-cppconst SoGtk SoGtkComponentAction
  (CREATION
   DESTRUCTION
   CHANGE))

(def-cppmethod SoGtk init
  (GtkWidget)
  ((string name)
   (string class (= "\"SoGtk\"")))
  (static #t))

(def-cppmethod SoGtk init
  (GtkWidget)
  ((int argc)
   (string argv)
   (string name)
   (string class (= "\"SoGtk\"")))
  (static #t))

(def-cppmethod SoGtk init
  none
  ((GtkWidget toplevel))
  (static #t))

(def-cppmethod SoGtk mainLoop
  none
  ()
  (static #t))

(def-cppmethod SoGtk exitMainLoop
  none
  ()
  (static #t))

(def-cppmethod SoGtk getTopLevelWidget
  (GtkWidget)
  ()
  (static #t))

(def-cppmethod SoGtk getShellWidget
  (GtkWidget)
  ((GtkWidget widget))
  (static #t))

(def-cppmethod SoGtk show
  none
  ((GtkWidget widget))
  (static #t))

(def-cppmethod SoGtk hide
  none
  ((GtkWidget widget))
  (static #t))

(def-cppmethod SoGtk setWidgetSize
  none
  ((GtkWidget widget)
   (SbVec2s size))
  (static #t))

(def-cppmethod SoGtk getWidgetSize
  (SbVec2s)
  ((GtkWidget widget))
  (static #t))

(def-cppmethod SoGtk createSimpleErrorDialog
  none
  ((GtkWidget widget)
   (string title)
   (string string-1)
   (string string-2 (= "NULL")))
  (static #t))

(def-cppmethod SoGtk addComponentActionCallback
  none
  ((callback proc))
  (static #t))

(def-cppmethod SoGtk removeComponentActionCallback
  none
  ((callback proc))
  (static #t))

;; -- SoGtkTypedObject -----------------------------------------------------

(def-cppclass SoGtkTypedObject ()
  (header "<Inventor/Gtk/SoGtkTypedObject.h>")
  (abstract #t))

(def-cppmethod SoGtkTypedObject init
  none
  ()
  (static #t))

(def-cppmethod SoGtkTypedObject getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkTypedObject getTypeId
  SoType
  ()
  (virtual #t)
  (abstract #t))

(def-cppmethod SoGtkTypedObject isOfType
  boolean
  ((SoType type)))

;; -- SoGtkDevice ----------------------------------------------------------

(def-cppclass SoGtkDevice (SoGtkTypedObject)
  (header "<Inventor/Gtk/devices/SoGtkDevice.h>")
  (abstract #t))

(def-cppdestr SoGtkDevice)

(def-cppmethod SoGtkDevice initClasses
  none
  ()
  (static #t))

(def-cppmethod SoGtkDevice initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkDevice getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkDevice getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkDevice enable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t)
  (abstract #t))

(def-cppmethod SoGtkDevice disable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t)
  (abstract #t))

(def-cppmethod SoGtkDevice translateEvent
  (SoEvent)
  ((GdkEvent event))
  (virtual #t)
  (abstract #t))

(def-cppmethod SoGtkDevice setWindowSize
  none
  ((SbVec2s size)))

(def-cppmethod SoGtkDevice getWindowSize
  (SbVec2s)
  ())

;; -- SoGtkMouse -----------------------------------------------------------

(def-cppclass SoGtkMouse (SoGtkDevice)
  (header "<Inventor/Gtk/devices/SoGtkMouse.h>"))

(def-cppconst SoGtkMouse SoGtkMouseEventMask
  (ButtonPressMask
   ButtonReleaseMask
   PointerMotionMask
   ButtonMotionMask
   SO_GTK_ALL_MOUSE_EVENTS))

(def-cppconstr SoGtkMouse
  ((int mask (= "SO_GTK_ALL_MOUSE_EVENTS") (mask SoGtkMouseEventMask))))

(def-cppdestr SoGtkMouse)

(def-cppmethod SoGtkMouse initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkMouse getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkMouse getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkMouse enable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t))

(def-cppmethod SoGtkMouse disable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t))

(def-cppmethod SoGtkMouse translateEvent
  (SoEvent)
  ((GdkEvent event))
  (virtual #t))

;; -- SoGtkKeyboard --------------------------------------------------------

(def-cppclass SoGtkKeyboard (SoGtkDevice)
  (header "<Inventor/Gtk/devices/SoGtkKeyboard.h>"))

(def-cppconst SoGtkKeyboard SoGtkKeyboardEventMask
  (KeyPressMask
   KeyReleaseMask
   SO_GTK_ALL_KEYBOARD_EVENTS))

(def-cppconstr SoGtkKeyboard
  ((int mask (= "SO_GTK_ALL_KEYBOARD_EVENTS") (mask SoGtkKeyboardEventMask))))

(def-cppdestr SoGtkKeyboard)

(def-cppmethod SoGtkKeyboard initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkKeyboard getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkKeyboard getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkKeyboard enable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t))

(def-cppmethod SoGtkKeyboard disable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t))

(def-cppmethod SoGtkKeyboard translateEvent
  (SoEvent)
  ((GdkEvent event))
  (virtual #t))

;; -- SoGtkInputFocus ------------------------------------------------------

(def-cppclass SoGtkInputFocus (SoGtkDevice)
  (header "<Inventor/Gtk/devices/SoGtkInputFocus.h>"))

(def-cppconst SoGtkInputFocus SoGtkInputFocusEventMask
  (EnterWindowMask
   LeaveWindowMask
   SOGTK_ALL_FOCUS_EVENTS))

(def-cppconstr SoGtkInputFocus
  ((int mask (= "SO_GTK_ALL_FOCUS_EVENTS"))))

(def-cppdestr SoGtkInputFocus)

(def-cppmethod SoGtkInputFocus initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkInputFocus getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkInputFocus getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkInputFocus enable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t))

(def-cppmethod SoGtkInputFocus disable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t))

(def-cppmethod SoGtkInputFocus translateEvent
  (SoEvent)
  ((GdkEvent event))
  (virtual #t))

;; -- SoGtkSpaceball -------------------------------------------------------

(def-cppclass SoGtkSpaceball (SoGtkDevice)
  (header "<Inventor/Gtk/devices/SoGtkSpaceball.h>"))

(def-cppconst SoGtkSpaceball SoGtkSpaceballEventMask
  (MOTION
   PRESS
   RELEASE
   ALL))

(def-cppconstr SoGtkSpaceball
  ((int mask (= "SoGtkSpaceball::ALL"))))

(def-cppdestr SoGtkSpaceball)

(def-cppmethod SoGtkSpaceball initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkSpaceball getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkSpaceball getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkSpaceball enable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t))

(def-cppmethod SoGtkSpaceball disable
  none
  ((GtkWidget widget)
   (callback proc))
  (virtual #t))

(def-cppmethod SoGtkSpaceball translateEvent
  (SoEvent)
  ((GdkEvent event))
  (virtual #t))

(def-cppmethod SoGtkSpaceball setRotationFactor
  none
  ((float factor)))

(def-cppmethod SoGtkSpaceball getRotationFactor
  (float)
  ())

(def-cppmethod SoGtkSpaceball setTranslationFactor
  none
  ((float factor)))

(def-cppmethod SoGtkSpaceball getTranslationFactor
  (float)
  ())

(def-cppmethod SoGtkSpaceball exists
  (boolean)
  ()
  (scm-name "exists?")
  (static #t))

(def-cppmethod SoGtkSpaceball setFocusToWindow
  none
  ((boolean flag)))

(def-cppmethod SoGtkSpaceball isFocusToWindow
  (boolean)
  ()
  (scm-name "focus-to-window?"))

;; -- SoGtkComponent -------------------------------------------------------

(def-cppclass SoGtkComponent (SoGtkTypedObject)
  (header "<Inventor/Gtk/SoGtkComponent.h>")
  (abstract #t))

(def-cppmethod SoGtkComponent initClasses
  none
  ()
  (static #t))

(def-cppmethod SoGtkComponent initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkComponent getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkComponent getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkComponent show
  none
  ()
  (virtual #t))

(def-cppmethod SoGtkComponent hide
  none
  ()
  (virtual #t))

(def-cppmethod SoGtkComponent isVisible
  (boolean)
  ()
  (scm-name "visible?"))

(def-cppmethod SoGtkComponent getWidget
  (GtkWidget)
  ()
  (const #t))

(def-cppmethod SoGtkComponent baseWidget
  (GtkWidget)
  ()
  (const #t))

(def-cppmethod SoGtkComponent isTopLevelShell
  (boolean)
  ()
  (scm-name "top-level-shell?")
  (const #t))

(def-cppmethod SoGtkComponent getShellWidget
  (GtkWidget)
  ()
  (const #t))

(def-cppmethod SoGtkComponent getParentWidget
  (GtkWidget)
  ()
  (const #t))

(def-cppmethod SoGtkComponent setSize
  none
  ((SbVec2s size)))

(def-cppmethod SoGtkComponent getSize
  (val SbVec2s)
  ()
  (const #t))

(def-cppmethod SoGtkComponent setTitle
  none
  ((string title)))

(def-cppmethod SoGtkComponent getTitle
  (string)
  ())

(def-cppmethod SoGtkComponent setIconTitle
  none
  ((string title)))

(def-cppmethod SoGtkComponent getIconTitle
  (string)
  ())

(def-cppmethod SoGtkComponent setWindowCloseCallback
  none
  ((callback proc)))

(def-cppmethod SoGtkComponent getWidgetName
  (string)
  ()
  (const #t))

(def-cppmethod SoGtkComponent getClassName
  (string)
  ()
  (const #t))

;; -- SoGtkGLWidget --------------------------------------------------------

(def-cppclass SoGtkGLWidget (SoGtkComponent)
  (header "<Inventor/Gtk/SoGtkGLWidget.h>")
  (abstract #t))

(def-cppconst SoGtkGLWidget glModes
  (SO_GL_RGB
   SO_GL_DOUBLE
   SO_GL_ZBUFFER
   SO_GL_OVERLAY
   SO_GL_STEREO))

(def-cppmethod SoGtkGLWidget initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkGLWidget getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkGLWidget getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkGLWidget setBorder
  none
  ((boolean border)))

(def-cppmethod SoGtkGLWidget isBorder
  (boolean)
  ()
  (scm-name "border?")
  (const #t))

(def-cppmethod SoGtkGLWidget setDoubleBuffer
  none
  ((boolean doublebuffer))
  (virtual #t))

(def-cppmethod SoGtkGLWidget isDoubleBuffer
  (boolean)
  ()
  (scm-name "double-buffer?")
  (const #t))

(def-cppmethod SoGtkGLWidget setDrawToFrontBufferEnable
  none
  ((boolean enable)))

; name should change in SoGui libraries
(def-cppmethod SoGtkGLWidget isDrawToFrontBufferEnable
  (boolean)
  ()
  (scm-name "draw-to-front-buffer?")
  (const #t))

;; -- SoGtkRenderArea ------------------------------------------------------

(def-cppclass SoGtkRenderArea (SoGtkGLWidget)
  (header "<Inventor/Gtk/SoGtkRenderArea.h>"))

(def-cppconstr SoGtkRenderArea
  ((GtkWidget parent (= "NULL"))
   (string name (= "NULL"))
   (boolean embed (= "FALSE"))
   (boolean mouse-input (= "TRUE"))
   (boolean keyboard-input (= "TRUE")))
  (scm-name "new-SoGtkRenderArea"))

(def-destr SoGtkRenderArea)

(def-cppmethod SoGtkRenderArea initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkRenderArea getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkRenderArea getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkRenderArea setSceneGraph
  none
  ((SoNode scene))
  (virtual #t))

(def-cppmethod SoGtkRenderArea getSceneGraph
  (SoNode)
  ()
  (virtual #t))

(def-cppmethod SoGtkRenderArea setOverlaySceneGraph
  none
  ((SoNode scene)))

(def-cppmethod SoGtkRenderArea getOverlaySceneGraph
  (SoNode)
  ())

(def-cppmethod SoGtkRenderArea registerDevice
  none
  ((SoGtkDevice device)))

(def-cppmethod SoGtkRenderArea unregisterDevice
  none
  ((SoGtkDevice device)))

(def-cppmethod SoGtkRenderArea setBackgroundColor
  none
  ((SbColor color (ref #t))))

(def-cppmethod SoGtkRenderArea getBackgroundColor
  (SbColor (ref #t))
  ()
  (const #t))

(def-cppmethod SoGtkRenderArea setBackgroundIndex
  none
  ((int index)))

(def-cppmethod SoGtkRenderArea getBackgroundIndex
  (int)
  ()
  (const #t))

(def-cppmethod SoGtkRenderArea setOverlayBackgroundIndex
  none
  ((int index)))

(def-cppmethod SoGtkRenderArea getOverlayBackgroundIndex
  (int)
  ()
  (const #t))

(def-cppmethod SoGtkRenderArea setViewportRegion
  none
  ((SbViewportRegion region (ref #t))))

(def-cppmethod SoGtkRenderArea getViewportRegion
  (SbViewportRegion (ref #t))
  ()
  (const #t))

(def-cppmethod SoGtkRenderArea setTransparencyType
  none
  ((int type)))

(def-cppmethod SoGtkRenderArea getTransparencyType
  (int)
  ()
  (const #t))

(def-cppmethod SoGtkRenderArea setAntialiasing
  none
  ((boolean smoothing)
   (int passes)))

(def-cppmethod SoGtkRenderArea getAntialiasing
  none
  ((boolean smoothing (ref #t) (out #t))
   (int passes (ref #t) (out #t)))
  (const #t))

(def-cppmethod SoGtkRenderArea setClearBeforeRender
  none
  ((boolean enable)
   (boolean zbEnable (= "TRUE"))))

(def-cppmethod SoGtkRenderArea isClearBeforeRender
  (boolean)
  ()
  (scm-name "clear-before-render?")
  (const #t))

(def-cppmethod SoGtkRenderArea isClearZBufferBeforeRender
  (boolean)
  ()
  (scm-name "clear-zbuffer-before-render?")
  (const #t))

(def-cppmethod SoGtkRenderArea setClearBeforeOverlayRender
  none
  ((boolean enable)))

(def-cppmethod SoGtkRenderArea isClearBeforeOverlayRender
  (boolean)
  ()
  (scm-name "clear-before-overlay-render?")
  (const #t))

(def-cppmethod SoGtkRenderArea setAutoRedraw
  none
  ((boolean enable)))

(def-cppmethod SoGtkRenderArea isAutoRedraw
  (boolean)
  ()
  (scm-name "auto-redraw?")
  (const #t))

(def-cppmethod SoGtkRenderArea setRedrawPriority
  none
  ((int priority)))

(def-cppmethod SoGtkRenderArea getRedrawPriority
  (int)
  ()
  (const #t))

(def-cppmethod SoGtkRenderArea getDefaultRedrawPriority
  (int)
  ()
  (static #t))

(def-cppmethod SoGtkRenderArea render
  none
  ())

(def-cppmethod SoGtkRenderArea renderOverlay
  none
  ())

(def-cppmethod SoGtkRenderArea scheduleRedraw
  none
  ())

(def-cppmethod SoGtkRenderArea scheduleOverlayRedraw
  none
  ())

(def-cppmethod SoGtkRenderArea redrawOnSelectionChange
  none
  ((SoSelection selection)))

(def-cppmethod SoGtkRenderArea redrawOnOverlaySelectionChange
  none
  ((SoSelection selection)))

(def-cppmethod SoGtkRenderArea setEventCallback
  none
  ((callback proc)))

(def-cppmethod SoGtkRenderArea setSceneManager
  none
  ((SoSceneManager scenemanager)))

(def-cppmethod SoGtkRenderArea getSceneManager
  (SoSceneManager)
  ()
  (const #t))

(def-cppmethod SoGtkRenderArea setOverlaySceneManager
  none
  ((SoSceneManager scenemanager)))

(def-cppmethod SoGtkRenderArea getOverlaySceneManager
  (SoSceneManager)
  ()
  (const #t))

(def-cppmethod SoGtkRenderArea setGLRenderAction
  none
  ((SoGLRenderAction renderaction))
  (scm-name "set-gl-render-action"))

(def-cppmethod SoGtkRenderArea getGLRenderAction
  (SoGLRenderAction)
  ()
  (scm-name "get-gl-render-action")
  (const #t))

(def-cppmethod SoGtkRenderArea setOverlayGLRenderAction
  none
  ((SoGLRenderAction renderaction))
  (scm-name "set-overlay-gl-render-action"))

(def-cppmethod SoGtkRenderAction getOverlayGLRenderAction
  (SoGLRenderAction)
  ()
  (scm-name "get-overlay-gl-render-action")
  (const #t))

;; -- SoGtkViewer ----------------------------------------------------------

(def-cppclass SoGtkViewer (SoGtkRenderArea)
  (header "<Inventor/Gtk/viewers/SoGtkViewer.h>"))

(def-cppconst SoGtkViewer Type
  (BROWSER
   EDITOR))

(def-cppconst SoGtkViewer DecimationStrategy
  (NORMAL
   FIXED_NUM_TRIANGLES
   FRAMES_PER_SECOND
   FIXED_PERCENTAGE))

(def-cppconst SoGtkViewer DrawStyle
  (VIEW_AS_IS
   VIEW_HIDDEN_LINE
   VIEW_NO_TEXTURE
   VIEW_LOW_COMPLEXITY
   VIEW_LINE
   VIEW_POINT
   VIEW_BBOX
   VIEW_LOW_RES_LINE
   VIEW_LOW_RES_POINT
   VIEW_SAME_AS_STILL))

(def-cppconst SoGtkViewer DrawType
  (STILL
   INTERACTIVE))

(def-cppconst SoGtkViewer BufferType
  (BUFFER_SINGLE
   BUFFER_DOUBLE
   BUFFER_INTERACTIVE))

(def-cppmethod SoGtkViewer initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkViewer getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkViewer getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkViewer setCamera
  none
  ((SoCamera camera))
  (virtual #t))

(def-cppmethod SoGtkViewer getCamera
  (SoCamera)
  ()
  (const #t))

(def-cppmethod SoGtkViewer setCameraType
  none
  ((SoType type))
  (virtual #t))

(def-cppmethod SoGtkViewer getCameraType
  (SoType)
  ()
  (const #t))

(def-cppmethod SoGtkViewer viewAll
  none
  ()
  (virtual #t))

(def-cppmethod SoGtkViewer saveHomePosition
  none
  ()
  (virtual #t))

(def-cppmethod SoGtkViewer resetToHomePosition
  none
  ()
  (virtual #t))

(def-cppmethod SoGtkViewer setHeadlight
  none
  ((boolean enable))
  (virtual #t))

(def-cppmethod SoGtkViewer isHeadlight
  (boolean)
  ()
  (scm-name "headlight?")
  (const #t))

(def-cppmethod SoGtkViewer getHeadlight
  (SoDirectionalLight)
  ()
  (const #t))

(def-cppmethod SoGtkViewer setDrawStyle
  none
  ((int type (choice DrawType))
   (int style (choice DrawStyle)))
  (virtual #t))

(def-cppmethod SoGtkViewer getDrawStyle
  (int (choice DrawStyle))
  ((int type (choice DrawType)))
  (const #t))

(def-cppmethod SoGtkViewer setBufferingType
  none
  ((int type (choice BufferType)))
  (virtual #t))

(def-cppmethod SoGtkViewer getBufferingType
  (int (choice BufferType))
  ()
  (const #t))

(def-cppmethod SoGtkViewer setViewing
  none
  ((boolean enable))
  (virtual))

(def-cppmethod SoGtkViewer isViewing
  (boolean)
  ()
  (scm-name "viewing?")
  (const #t))

(def-cppmethod SoGtkViewer setCursorEnabled
  none
  ((boolean enable))
  (virtual #t))

(def-cppmethod SoGtkViewer isCursorEnabled
  (boolean)
  ()
  (scm-name "cursor-enabled?")
  (const #t))

(def-cppmethod SoGtkViewer setAutoClipping
  none
  ((boolean enable)))

(def-cppmethod SoGtkViewer isAutoClipping
  (boolean)
  ()
  (scm-name "auto-clipping?")
  (const #t))

(def-cppmethod SoGtkViewer setStereoViewing
  none
  ((boolean enable))
  (virtual #t))

(def-cppmethod SoGtkViewer isStereoViewing
  (boolean)
  ()
  (scm-name "stereo-viewing?")
  (const #t))

(def-cppmethod SoGtkViewer setStereoOffset
  none
  ((float distance))
  (virtual #t))

(def-cppmethod SoGtkViewer getStereoOffset
  (float)
  ()
  (const #t))

(def-cppmethod SoGtkViewer setDetailSeek
  none
  ((boolean enable)))

(def-cppmethod SoGtkViewer isDetailSeek
  (boolean)
  ()
  (scm-name "detail-seek?")
  (const #t))

(def-cppmethod SoGtkViewer setSeekTime
  none
  ((float seconds)))

(def-cppmethod SoGtkViewer getSeekTime
  (float)
  ()
  (const #t))

(def-cppmethod SoGtkViewer addStartCallback
  none
  ((callback proc)))

(def-cppmethod SoGtkViewer addFinishCallback
  none
  ((callback proc)))

(def-cppmethod SoGtkViewer removeStartCallback
  none
  ((callback proc)))

(def-cppmethod SoGtkViewer removeFinishCallback
  none
  ((callback proc)))

(def-cppmethod SoGtkViewer copyView
  none
  ((SbTime eventtime)))

(def-cppmethod SoGtkViewer pasteView
  none
  ((SbTime eventtime)))

(def-cppmethod SoGtkViewer recomputeSceneSize
  none
  ())

(def-cppmethod SoGtkViewer setDecimationStrategy
  none
  ((int strategy (choice DecimationStrategy))))

(def-cppmethod SoGtkViewer getDecimationStrategy
  (int (choice DecimationStrategy))
  ()
  (const #t))

(def-cppmethod SoGtkViewer setGoalNumberOfTriangles
  none
  ((int goal)))

(def-cppmethod SoGtkViewer getGoalNumberOfTriangles
  (int)
  ()
  (const #t))

(def-cppmethod SoGtkViewer setGoalFramesPerSecond
  none
  ((float goal)))

(def-cppmethod SoGtkViewer getGoalFramesPerSecond
  (float)
  ()
  (const #t))

(def-cppmethod SoGtkViewer setFixedPercentage
  none
  ((float percentage)))

(def-cppmethod SoGtkViewer getFixedPercentage
  (float)
  ()
  (const #t))

(def-cppmethod SoGtkViewer enableFullRenderingWhenStill
  none
  ((boolean enable)))

(def-cppmethod SoGtkViewer isFullRenderingWhenStill
  (boolean)
  ()
  (scm-name "full-rendering-when-still?")
  (const #t))

(def-cppmethod SoGtkViewer isStillNow
  (boolean)
  ()
  (scm-name "still-now?")
  (const #t))

(def-cppmethod SoGtkViewer setFramesPerSecondCallback
  none
  ((callback proc)))

(def-cppmethod SoGtkViewer setNumSamples
  none
  ((int numFrames)))

(def-cppmethod SoGtkViewer getNumSamples
  (int)
  ()
  (const #t))

(def-cppmethod SoGtkViewer setDecimationPercentageCallback
  none
  ((callback proc)))

(def-cppmethod SoGtkViewer getCurrentDecimationPercentage
  (float)
  ()
  (const #t))

(def-cppmethod SoGtkViewer setDoubleBuffer
  none
  ((boolean enable))
  (virtual #t)
  (overload #t))

(def-cppmethod SoGtkViewer setSceneGraph
  none
  ((SoNode root))
  (virtual #t)
  (overload #t))

(def-cppmethod SoGtkViewer getSceneGraph
  (SoNode)
  ()
  (virtual #t)
  (overload #t))

;; -- SoGtkFullViewer ------------------------------------------------------

(def-cppclass SoGtkFullViewer (SoGtkViewer)
  (header "<Inventor/Gtk/viewer/SoGtkFullViewer.h>"))

(def-cppconst SoGtkFullViewer BuildFlag
  (BUILD_NONE
   BUILD_DECORATION
   BUILD_POPUP
   BUILD_ALL))

(def-cppmethod SoGtkFullViewer initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkFullViewer getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkFullViewer getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkFullViewer setDecoration
  none
  ((boolean enable)))

(def-cppmethod SoGtkFullViewer isDecoration
  (boolean)
  ()
  (scm-name "decoration?")
  (const #t))

(def-cppmethod SoGtkFullViewer setPopupMenuEnabled
  none
  ((boolean enable)))

(def-cppmethod SoGtkFullViewer isPopupMenuEnabled
  (boolean)
  ()
  (scm-name "popup-menu-enabled?")
  (const #t))

(def-cppmethod SoGtkFullViewer getAppPushButtonParent
  (ptr GtkWidget)
  ()
  (const #t))

(def-cppmethod SoGtkFullViewer addAppPushButton
  none
  ((ptr GtkWidget widget)))

(def-cppmethod SoGtkFullViewer insertAppPushButton
  none
  ((GtkWidget button)
   (int index)))

(def-cppmethod SoGtkFullViewer removeAppPushButton
  none
  ((GtkWidget button)))

(def-cppmethod SoGtkFullViewer findAppPushButton
  (int)
  ((GtkWidget button))
  (const #t))

(def-cppmethod SoGtkFullViewer lengthAppPushButton
  (int)
  ()
  (const #t))

(def-cppmethod SoGtkFullViewer getRenderAreaWidget
  (GtkWidget)
  ())

(def-cppmethod SoGtkFullViewer setViewing
  none
  ((boolean enable))
  (virtual #t)
  (overload #t))

(def-cppmethod SoGtkFullViewer setHeadlight
  none
  ((boolean enable))
  (virtual #t)
  (overload #t))

(def-cppmethod SoGtkFullViewer setDrawStyle
  none
  ((int type (choice DrawType))
   (int style (choice DrawStyle)))
  (virtual #t)
  (overload #t))

(def-cppmethod SoGtkFullViewer setBufferingType
  none
  ((int type (choice BufferType)))
  (virtual #t)
  (overload #t))

(def-cppmethod SoGtkFullViewer setCamera
  none
  ((SoCamera camera))
  (virtual #t)
  (overload #t))

(def-cppmethod SoGtkFullViewer hide
  none
  ()
  (virtual #t)
  (overload #t))

;; -- SoGtkExaminerViewer --------------------------------------------------

(def-cppclass SoGtkExaminerViewer (SoGtkFullViewer)
  (header "<Inventor/Gtk/viewer/SoGtkExaminerViewer.h>"))

(def-cppconstr SoGtkExaminerViewer
  ((GtkWidget parent (= "NULL"))
   (string name (= "NULL"))
   (boolean embed (= "TRUE"))
   (int flags (= "BUILD_ALL"))
   (int type (= "BROWSER"))))

(def-cppdestr SoGtkExaminerViewer)

(def-cppmethod SoGtkExaminerViewer initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkExaminerViewer getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkExaminerViewer getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkExaminerViewer setViewing
  none
  ((boolean enable))
  (virtual #t)
  (overload #t))

(def-cppmethod SoGtkExaminerViewer setAnimationEnabled
  none
  ((boolean enable)))

(def-cppmethod SoGtkExaminerViewer isAnimationEnable
  (boolean)
  ()
  (scm-name "animation-enable?")
  (const #t))

(def-cppmethod SoGtkExaminerViewer stopAnimating
  none
  ())

(def-cppmethod SoGtkExaminerViewer isAnimating
  (boolean)
  ()
  (scm-name "animating?")
  (const #t))

(def-cppmethod SoGtkExaminerViewer setFeedbackVisibility
  none
  ((boolean enable)))

(def-cppmethod SoGtkExaminerViewer isFeedbackVisible
  (boolean)
  ()
  (scm-name "feedback-visible?")
  (const #t))

(def-cppmethod SoGtkExaminerViewer setFeedbackSize
  none
  ((int size)))

(def-cppmethod SoGtkExaminerViewer getFeedbackSize
  (int)
  ()
  (const #t))

(def-cppmethod SoGtkExaminerViewer setCamera
  none
  ((SoCamera camera))
  (virtual #t)
  (overload #t))

;; -- SoGtkCustomViewer ----------------------------------------------------

(def-cppclass SoGtkCustomViewer (SoGtkViewer)
  (header "<Inventor/Gtk/viewers/SoGtkCustomViewer.h>"))

(def-cppmethod SoGtkCustomViewer initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkCustomViewer getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkCustomViewer getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkCustomViewer getMenuBar
  (GtkWidget)
  ())

(def-cppmethod SoGtkCustomViewer getStatusBar
  (GtkWidget)
  ())

(def-cppmethod SoGtkCustomViewer getDirector
  (SoAnyDirector)
  ())

(def-cppmethod SoGtkCustomViewer setInteractionMode
  none
  ((int mode (choice))))

(def-cppmethod SoGtkCustomViewer getInteractionMode
  (choice)
  ())

(def-cppmethod SoGtkCustomViewer setFPSCount
  none
  ((float fps))
  (scm-name "set-fps-count"))

(def-cppmethod SoGtCustomViewer getFPSCount
  (float)
  ()
  (scm-name "get-fps-count"))

;; -- SoGtkRoster ----------------------------------------------------------

(def-cppclass SoGtkRoster (SoGtkComponent)
  (header "<Inventor/Gtk/SoGtkRoster.h>"))

(def-cppconstr SoGtkRoster
  ((GtkWidget parent (= "NULL"))
   (string name (= "NULL"))
   (boolean embed (= "TRUE"))))

(def-cppdestr SoGtkRoster)

(def-cppmethod SoGtkRoster initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkRoster getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkRoster getTypeId
  SoType
  ()
  (virtual #t))

;; -- SoGtkGraphEditor -----------------------------------------------------

(def-cppclass SoGtkGraphEditor (SoGtkComponent)
  (header "<Inventor/Gtk/SoGtkGraphEditor.h>"))

(def-cppconstr SoGtkGraphEditor
  ((GtkWidget parent (= "NULL"))
   (string name (= "NULL"))
   (boolean embed (= "TRUE"))
   (int parts (= "EVERYTHING"))))

(def-cppdestr SoGtkGraphEditor)

(def-cppmethod SoGtkGraphEditor initClass
  none
  ()
  (static #t))

(def-cppmethod SoGtkGraphEditor getClassTypeId
  SoType
  ()
  (static #t))

(def-cppmethod SoGtkGraphEditor getTypeId
  SoType
  ()
  (virtual #t))

(def-cppmethod SoGtkGraphEditor setSceneGraph
  none
  ((SoNode root)))

(def-cppmethod SoGtkGraphEditor getSceneGraph
  (SoNode)
  ())

;; eof
