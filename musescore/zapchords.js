//=============================================================================
//  MuseScore
//  Linux Music Score Editor
//  $Id:$
//
//  Zap Chords plugin
//
//  Copyright (C)2009 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================


//
// error handling is mostly omitted!
//

//---------------------------------------------------------
//    init
//---------------------------------------------------------

function init()
      {
      };

var form;

//---------------------------------------------------------
//    run
//    remove all but the first notes from all chords
//---------------------------------------------------------

function run()
{
      var score = curScore;
      var cursor = new Cursor(curScore);
      var selectionEnd = new Cursor(curScore);
      cursor.goToSelectionStart();
      selectionEnd.goToSelectionEnd();

      var startStaff = cursor.staff;
      var endStaff = selectionEnd.staff;

      for (var staff = startStaff; staff<endStaff; ++staff) {
          cursor.goToSelectionStart();
          for (var v=0; v<4; v++) {
              cursor.goToSelectionStart();
              cursor.voice = v;
              cursor.staff = staff;

              while (cursor.tick() < selectionEnd.tick()) {
                    if (cursor.isChord()) {
                       var chord = cursor.chord();
                       if (chord.notes>1) {
                           for (var i = 0; i<chord.notes; i++) {
                              var note = chord.note(i);
                              chord.removeNote(i);
                           } 
                        }
                    }
              cursor.next();
              }
        }
      }
}
var mscorePlugin = {
      majorVersion: 0,
      minorVersion: 1,
      menu: 'Plugins.Zap Chords',
      init: init,
      run:  run
      };

mscorePlugin;

