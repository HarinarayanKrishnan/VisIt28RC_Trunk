/*****************************************************************************
*
* Copyright (c) 2000 - 2014, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#ifndef APPEARANCEATTRIBUTES_H
#define APPEARANCEATTRIBUTES_H
#include <state_exports.h>
#include <string>
#include <AttributeSubject.h>


// ****************************************************************************
// Class: AppearanceAttributes
//
// Purpose:
//    This class contains the GUI/viewer appearance attributes.
//
// Notes:      Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

class STATE_API AppearanceAttributes : public AttributeSubject
{
public:
    // These constructors are for objects of this class
    AppearanceAttributes();
    AppearanceAttributes(const AppearanceAttributes &obj);
protected:
    // These constructors are for objects derived from this class
    AppearanceAttributes(private_tmfs_t tmfs);
    AppearanceAttributes(const AppearanceAttributes &obj, private_tmfs_t tmfs);
public:
    virtual ~AppearanceAttributes();

    virtual AppearanceAttributes& operator = (const AppearanceAttributes &obj);
    virtual bool operator == (const AppearanceAttributes &obj) const;
    virtual bool operator != (const AppearanceAttributes &obj) const;
private:
    void Init();
    void Copy(const AppearanceAttributes &obj);
public:

    virtual const std::string TypeName() const;
    virtual bool CopyAttributes(const AttributeGroup *);
    virtual AttributeSubject *CreateCompatible(const std::string &) const;
    virtual AttributeSubject *NewInstance(bool) const;

    // Property selection methods
    virtual void SelectAll();
    void SelectBackground();
    void SelectForeground();
    void SelectFontName();
    void SelectStyle();
    void SelectDefaultForeground();
    void SelectDefaultBackground();
    void SelectDefaultFontName();
    void SelectDefaultStyle();

    // Property setting methods
    void SetUseSystemDefault(bool useSystemDefault_);
    void SetBackground(const std::string &background_);
    void SetForeground(const std::string &foreground_);
    void SetFontName(const std::string &fontName_);
    void SetStyle(const std::string &style_);
    void SetOrientation(int orientation_);
    void SetDefaultForeground(const std::string &defaultForeground_);
    void SetDefaultBackground(const std::string &defaultBackground_);
    void SetDefaultFontName(const std::string &defaultFontName_);
    void SetDefaultStyle(const std::string &defaultStyle_);
    void SetDefaultOrientation(int defaultOrientation_);

    // Property getting methods
    bool              GetUseSystemDefault() const;
    const std::string &GetBackground() const;
          std::string &GetBackground();
    const std::string &GetForeground() const;
          std::string &GetForeground();
    const std::string &GetFontName() const;
          std::string &GetFontName();
    const std::string &GetStyle() const;
          std::string &GetStyle();
    int               GetOrientation() const;
    const std::string &GetDefaultForeground() const;
          std::string &GetDefaultForeground();
    const std::string &GetDefaultBackground() const;
          std::string &GetDefaultBackground();
    const std::string &GetDefaultFontName() const;
          std::string &GetDefaultFontName();
    const std::string &GetDefaultStyle() const;
          std::string &GetDefaultStyle();
    int               GetDefaultOrientation() const;

    // Persistence methods
    virtual bool CreateNode(DataNode *node, bool completeSave, bool forceAdd);
    virtual void SetFromNode(DataNode *node);


    // Keyframing methods
    virtual std::string               GetFieldName(int index) const;
    virtual AttributeGroup::FieldType GetFieldType(int index) const;
    virtual std::string               GetFieldTypeName(int index) const;
    virtual bool                      FieldsEqual(int index, const AttributeGroup *rhs) const;

    // User-defined methods
    void InitializeStyle();

    // IDs that can be used to identify fields in case statements
    enum {
        ID_useSystemDefault = 0,
        ID_background,
        ID_foreground,
        ID_fontName,
        ID_style,
        ID_orientation,
        ID_defaultForeground,
        ID_defaultBackground,
        ID_defaultFontName,
        ID_defaultStyle,
        ID_defaultOrientation,
        ID__LAST
    };

private:
    bool        useSystemDefault;
    std::string background;
    std::string foreground;
    std::string fontName;
    std::string style;
    int         orientation;
    std::string defaultForeground;
    std::string defaultBackground;
    std::string defaultFontName;
    std::string defaultStyle;
    int         defaultOrientation;

    // Static class format string for type map.
    static const char *TypeMapFormatString;
    static const private_tmfs_t TmfsStruct;
};
#define APPEARANCEATTRIBUTES_TMFS "bssssissssi"

#endif
