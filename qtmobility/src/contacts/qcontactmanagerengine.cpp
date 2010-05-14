/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qcontactmanagerengine.h"

#include "qcontactdetaildefinition.h"
#include "qcontactdetailfielddefinition.h"
#include "qcontactdetails.h"
#include "qcontactsortorder.h"
#include "qcontactfilters.h"
#include "qcontactabstractrequest.h"
#include "qcontactabstractrequest_p.h"
#include "qcontactrequests.h"
#include "qcontactrequests_p.h"
#include "qcontact.h"
#include "qcontactfetchhint.h"

#include "qcontact_p.h"
#include "qcontactdetail_p.h"

QTM_BEGIN_NAMESPACE

/*!
  \class QContactManagerEngine
  \preliminary
  \brief The QContactManagerEngine class provides the interface for all
  implementations of the contact manager backend functionality.
  \ingroup contacts-backends

  Instances of this class are usually provided by a
  \l QContactManagerEngineFactory, which is loaded from a plugin.

  The default implementation of this interface provides a basic
  level of functionality for some functions so that specific engines
  can simply implement the functionality that is supported by
  the specific contacts engine that is being adapted.

  More information on writing a contacts engine plugin is TODO.

  \sa QContactManager, QContactManagerEngineFactory
 */

/*!
  \fn QContactManagerEngine::QContactManagerEngine()

  A default, empty constructor.
 */

/*!
  \fn QContactManagerEngine::dataChanged()

  This signal is emitted some time after changes occur to the data managed by this
  engine, and the engine is unable to determine which changes occurred, or if the
  engine considers the changes to be radical enough to require clients to reload all data.

  If this signal is emitted, no other signals may be emitted for the associated changes.

  As it is possible that other processes (or other devices) may have caused the
  changes, the timing can not be determined.

  \sa contactsAdded(), contactsChanged(), contactsRemoved()
 */

/*!
  \fn QContactManagerEngine::contactsAdded(const QList<QContactLocalId>& contactIds);

  This signal is emitted some time after a set of contacts has been added to
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have added the contacts, the timing cannot be determined.

  The list of ids of contacts added is given by \a contactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::contactsChanged(const QList<QContactLocalId>& contactIds);

  This signal is emitted some time after a set of contacts has been modified in
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have modified the contacts, the timing cannot be determined.

  The list of ids of changed contacts is given by \a contactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::contactsRemoved(const QList<QContactLocalId>& contactIds);

  This signal is emitted some time after a set of contacts has been removed from
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have removed the contacts, the timing cannot be determined.

  The list of ids of removed contacts is given by \a contactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::relationshipsAdded(const QList<QContactLocalId>& affectedContactIds);

  This signal is emitted some time after a set of contacts has been added to
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have added the contacts, the timing cannot be determined.

  The list of ids of affected contacts is given by \a affectedContactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::relationshipsRemoved(const QList<QContactLocalId>& affectedContactIds);

  This signal is emitted some time after a set of relationships has been removed from
  this engine where the \l dataChanged() signal was not emitted for those changes.
  As it is possible that other processes (or other devices) may
  have removed the relationships, the timing cannot be determined.

  The list of ids of affected contacts is given by \a affectedContactIds.  There may be one or more
  ids in the list.

  \sa dataChanged()
 */

/*!
  \fn QContactManagerEngine::selfContactIdChanged(const QContactLocalId& oldId, const QContactLocalId& newId)

  This signal is emitted at some point after the id of the self-contact is changed from \a oldId to \a newId in the manager.
  If the \a newId is the invalid, zero id, then the self contact was deleted or no self contact exists.
  This signal must not be emitted if the dataChanged() signal was previously emitted for this change.
  As it is possible that other processes (or other devices) may
  have removed or changed the self contact, the timing cannot be determined.

  \sa dataChanged()
 */

/*! Returns the manager name for this QContactManagerEngine */
QString QContactManagerEngine::managerName() const
{
    return QString(QLatin1String("base"));
}

/*!
  Returns the parameters with which this engine was constructed.  Note that
  the engine may have discarded unused or invalid parameters at the time of
  construction, and these will not be returned.
 */
QMap<QString, QString> QContactManagerEngine::managerParameters() const
{
    return QMap<QString, QString>(); // default implementation requires no parameters.
}

/*!
  Returns the unique URI of this manager, which is built from the manager name and the parameters
  used to construct it.
 */
QString QContactManagerEngine::managerUri() const
{
    return QContactManager::buildUri(managerName(), managerParameters());
}

/*!
  Returns a list of contact ids that match the given \a filter, sorted according to the given list of \a sortOrders.
  Depending on the backend, this filtering operation may involve retrieving all the contacts.
  Any error which occurs will be saved in \a error.
 */
QList<QContactLocalId> QContactManagerEngine::contactIds(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, QContactManager::Error* error) const
{
    Q_UNUSED(filter);
    Q_UNUSED(sortOrders);

    *error = QContactManager::NotSupportedError;
    return QList<QContactLocalId>();
}

/*!
  Returns the list of contacts which match the given \a filter stored in the manager sorted according to the given list of \a sortOrders.

  Any operation error which occurs will be saved in \a error.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details and relationships
  in the matching contacts will be returned.  A client should not make changes to a contact which has
  been retrieved using a fetch hint other than the default fetch hint.  Doing so will result in information
  loss when saving the contact back to the manager (as the "new" restricted contact will
  replace the previously saved contact in the backend).

  \sa QContactFetchHint
 */
QList<QContact> QContactManagerEngine::contacts(const QContactFilter& filter, const QList<QContactSortOrder>& sortOrders, const QContactFetchHint& fetchHint, QContactManager::Error* error) const
{
    Q_UNUSED(filter);
    Q_UNUSED(sortOrders);
    Q_UNUSED(fetchHint);
    *error = QContactManager::NotSupportedError;
    return QList<QContact>();
}

/*!
  Returns the contact in the database identified by \a contactId.

  If the contact does not exist, an empty, default constructed QContact will be returned,
  and the \a error will be set to  \c QContactManager::DoesNotExistError.

  Any operation error which occurs will be saved in \a error.

  The \a fetchHint parameter describes the optimization hints that a manager may take.
  If the \a fetchHint is the default constructed hint, all existing details and relationships
  in the matching contact will be returned.  A client should not make changes to a contact which has
  been retrieved using a fetch hint other than the default fetch hint.  Doing so will result in information
  loss when saving the contact back to the manager (as the "new" restricted contact will
  replace the previously saved contact in the backend).

  \sa QContactFetchHint
 */
QContact QContactManagerEngine::contact(const QContactLocalId& contactId, const QContactFetchHint& fetchHint, QContactManager::Error* error) const
{
    Q_UNUSED(contactId);
    Q_UNUSED(fetchHint);
    *error = QContactManager::NotSupportedError;
    return QContact();
}

/*!
  Sets the id of the "self" contact to the given \a contactId.
  Returns true if the "self" contact id was set successfully.
  If the given \a contactId does not identify a contact
  stored in this manager, the \a error will be set to
  \c QContactManager::DoesNotExistError and the function will
  return false; if the backend does not support the
  concept of a "self" contact, the \a error will be set to
  \c QContactManager::NotSupportedError and the function will
  return false.
 */
bool QContactManagerEngine::setSelfContactId(const QContactLocalId& contactId, QContactManager::Error* error)
{
    Q_UNUSED(contactId);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Returns the id of the "self" contact which has previously been set.
  If no "self" contact has been set, or if the self contact was removed
  from the manager after being set, or if the backend does not support
  the concept of a "self" contact, an invalid id will be returned
  and the \a error will be set to \c QContactManager::DoesNotExistError.
 */
QContactLocalId QContactManagerEngine::selfContactId(QContactManager::Error* error) const
{
    *error = QContactManager::DoesNotExistError;
    return QContactLocalId();
}

/*!
  Returns a list of relationships of the given \a relationshipType in which the contact identified by the given \a participantId participates in the given \a role.
  If \a participantId is the default-constructed id, \a role is ignored and all relationships of the given \a relationshipType are returned.
  If \a relationshipType is empty, relationships of any type are returned.
  If no relationships of the given \a relationshipType in which the contact identified by the given \a participantId is involved in the given \a role exists,
  \a error is set to QContactManager::DoesNotExistError.
 */
QList<QContactRelationship> QContactManagerEngine::relationships(const QString& relationshipType, const QContactId& participantId, QContactRelationship::Role role, QContactManager::Error* error) const
{
    Q_UNUSED(relationshipType);
    Q_UNUSED(participantId);
    Q_UNUSED(role);

    *error = QContactManager::NotSupportedError;
    return QList<QContactRelationship>();
}

/*!
  Saves the given \a relationships in the database and returns true if the operation was successful.
  For any relationship which was unable to be saved, an entry into the \a errorMap will be created,
  with the key being the index into the input relationships list, and the value being the error which
  occurred for that index.

  The overall operation error will be saved in \a error.
 */
bool QContactManagerEngine::saveRelationships(QList<QContactRelationship>* relationships, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(relationships);
    Q_UNUSED(errorMap);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Saves the given \a relationship in the database.  If the relationship already exists in the database, this function will
  return \c false and the \a error will be set to \c QContactManager::AlreadyExistsError.
  If the relationship is saved successfully, this function will return \c true and \a error will be set
  to \c QContactManager::NoError.  Note that relationships cannot be updated directly using this function; in order
  to update a relationship, you must remove the old relationship, make the required modifications, and then save it.

  The given relationship is invalid if it is circular (the first contact is the second contact), or
  if it references a non-existent local contact (either the first or second contact).  If the given \a relationship is invalid,
  the function will return \c false and the \a error will be set to \c QContactManager::InvalidRelationshipError.

  The default implementation of this function converts the argument into a call to saveRelationships.
 */
bool QContactManagerEngine::saveRelationship(QContactRelationship *relationship, QContactManager::Error *error)
{
    // Convert to a list op
    if (relationship) {
        QList<QContactRelationship> list;
        list.append(*relationship);

        QMap<int, QContactManager::Error> errors;
        bool ret = saveRelationships(&list, &errors, error);

        if (errors.count() > 0)
            *error = errors.begin().value();

        *relationship = list.value(0);
        return ret;
    } else {
        *error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Removes the given \a relationship from the manager.  If the relationship exists in the manager, the relationship
  will be removed, the \a error will be set to \c QContactManager::NoError and this function will return true.  If no such
  relationship exists in the manager, the \a error will be set to \c QContactManager::DoesNotExistError and this function
  will return false.

  The default implementation of this function converts the argument into a call to removeRelationships
 */
bool QContactManagerEngine::removeRelationship(const QContactRelationship& relationship, QContactManager::Error* error)
{
    // Convert to a list op
    QList<QContactRelationship> list;
    list.append(relationship);

    QMap<int, QContactManager::Error> errors;
    bool ret = removeRelationships(list, &errors, error);

    if (errors.count() > 0)
        *error = errors.begin().value();

    return ret;
}


/*!
  Removes the given \a relationships from the database and returns true if the operation was successful.
  For any relationship which was unable to be removed, an entry into the \a errorMap will be created,
  with the key being the index into the input relationships list, and the value being the error which
  occurred for that index.

  The overall operation error will be saved in \a error.
 */
bool QContactManagerEngine::removeRelationships(const QList<QContactRelationship>& relationships, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(relationships);
    Q_UNUSED(errorMap);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Synthesizes the display label of the given \a contact in a platform specific manner.
  Any error that occurs will be stored in \a error.
  Returns the synthesized display label.
 */
QString QContactManagerEngine::synthesizedDisplayLabel(const QContact& contact, QContactManager::Error* error) const
{
    // synthesize the display name from the name of the contact, or, failing that, the organisation of the contact.
    *error = QContactManager::NoError;
    QList<QContactDetail> allNames = contact.details(QContactName::DefinitionName);

    const QLatin1String space(" ");

    // synthesize the display label from the name.
    for (int i=0; i < allNames.size(); i++) {
        const QContactName& name = allNames.at(i);

        if (!name.customLabel().isEmpty()) {
            // default behaviour is to allow the user to define a custom display label.
            return name.customLabel();
        }

        QString result;
        if (!name.value(QContactName::FieldPrefix).trimmed().isEmpty()) {
           result += name.value(QContactName::FieldPrefix);
        }

        if (!name.value(QContactName::FieldFirstName).trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldFirstName);
        }

        if (!name.value(QContactName::FieldMiddleName).trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldMiddleName);
        }

        if (!name.value(QContactName::FieldLastName).trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldLastName);
        }

        if (!name.value(QContactName::FieldSuffix).trimmed().isEmpty()) {
            if (!result.isEmpty())
                result += space;
            result += name.value(QContactName::FieldSuffix);
        }

        if (!result.isEmpty()) {
            return result;
        }
    }

    /* Well, we had no non empty names. if we have orgs, fall back to those */
    QList<QContactDetail> allOrgs = contact.details(QContactOrganization::DefinitionName);
    for (int i=0; i < allOrgs.size(); i++) {
        const QContactOrganization& org = allOrgs.at(i);
        if (!org.name().isEmpty()) {
            return org.name();
        }
    }

    *error = QContactManager::UnspecifiedError;
    return QString();
}

/*!
  Sets the contact display label of \a contact to the supplied \a displayLabel.

  This function does not touch the database in any way, and is purely a convenience to allow engine implementations to set the display label.
 */
void QContactManagerEngine::setContactDisplayLabel(QContact* contact, const QString& displayLabel)
{
    QContactDisplayLabel dl;
    dl.setValue(QContactDisplayLabel::FieldLabel, displayLabel);
    setDetailAccessConstraints(&dl, QContactDetail::Irremovable | QContactDetail::ReadOnly);
    contact->d->m_details.replace(0, dl);
}

/*!
  Returns true if the given \a feature is supported by this engine for contacts of the given \a contactType
 */
bool QContactManagerEngine::hasFeature(QContactManager::ManagerFeature feature, const QString& contactType) const
{
    Q_UNUSED(feature);
    Q_UNUSED(contactType);

    return false;
}

/*!
  Given an input \a filter, returns the canonical version of the filter.

  Some of the following transformations may be applied:
  \list
   \o Any QContactInvalidFilters contained in a union filter will be removed
   \o Any default QContactFilters contained in an intersection filter will be removed
   \o Any QContactIntersectionFilters with a QContactInvalidFilter contained will be
     replaced with a QContactInvalidFilter
   \o Any QContactUnionFilters with a default QContactFilter contained will be replaced
     with a default QContactFilter
   \o An empty QContactIntersectionFilter will be replaced with a QContactDefaultFilter
   \o An empty QContactUnionFilter will be replaced with a QContactInvalidFilter
   \o An empty QContactLocalIdFilter will be replaced with a QContactInvalidFilter
   \o An intersection or union filter with a single entry will be replaced by that entry
   \o A QContactDetailFilter or QContactDetailRangeFilter with no definition name will be replaced with a QContactInvalidFilter
   \o A QContactDetailRangeFilter with no range specified will be converted to a QContactDetailFilter
  \endlist
*/
QContactFilter QContactManagerEngine::canonicalizedFilter(const QContactFilter &filter)
{
    switch(filter.type()) {
        case QContactFilter::IntersectionFilter:
        {
            QContactIntersectionFilter f(filter);
            QList<QContactFilter> filters = f.filters();
            QList<QContactFilter>::iterator it = filters.begin();

            // XXX in theory we can remove duplicates in a set filter
            while (it != filters.end()) {
                QContactFilter canon = canonicalizedFilter(*it);
                if (canon.type() == QContactFilter::DefaultFilter) {
                    it = filters.erase(it);
                } else if (canon.type() == QContactFilter::InvalidFilter) {
                    return QContactInvalidFilter();
                } else {
                    *it = canon;
                    ++it;
                }
            }

            if (filters.count() == 0)
                return QContactFilter();
            if (filters.count() == 1)
                return filters.first();

            f.setFilters(filters);
            return f;
        }
        // unreachable

        case QContactFilter::UnionFilter:
        {
            QContactUnionFilter f(filter);
            QList<QContactFilter> filters = f.filters();
            QList<QContactFilter>::iterator it = filters.begin();

            // XXX in theory we can remove duplicates in a set filter
            while (it != filters.end()) {
                QContactFilter canon = canonicalizedFilter(*it);
                if (canon.type() == QContactFilter::InvalidFilter) {
                    it = filters.erase(it);
                } else if (canon.type() == QContactFilter::DefaultFilter) {
                    return QContactFilter();
                } else {
                    *it = canon;
                    ++it;
                }
            }

            if (filters.count() == 0)
                return QContactInvalidFilter();
            if (filters.count() == 1)
                return filters.first();

            f.setFilters(filters);
            return f;
        }
        // unreachable

        case QContactFilter::LocalIdFilter:
        {
            QContactLocalIdFilter f(filter);
            if (f.ids().count() == 0)
                return QContactInvalidFilter();
        }
        break; // fall through to return at end

        case QContactFilter::ContactDetailRangeFilter:
        {
            QContactDetailRangeFilter f(filter);
            if (f.detailDefinitionName().isEmpty())
                return QContactInvalidFilter();
            if (f.minValue() == f.maxValue()
                && f.rangeFlags() == (QContactDetailRangeFilter::ExcludeLower | QContactDetailRangeFilter::ExcludeUpper))
                return QContactInvalidFilter();
            if ((f.minValue().isNull() && f.maxValue().isNull()) || (f.minValue() == f.maxValue())) {
                QContactDetailFilter df;
                df.setDetailDefinitionName(f.detailDefinitionName(), f.detailFieldName());
                df.setMatchFlags(f.matchFlags());
                df.setValue(f.minValue());
                return df;
            }
        }
        break; // fall through to return at end

        case QContactFilter::ContactDetailFilter:
        {
            QContactDetailFilter f(filter);
            if (f.detailDefinitionName().isEmpty())
                return QContactInvalidFilter();
        }
        break; // fall through to return at end

        default:
            break; // fall through to return at end
    }
    return filter;
}


/*!
  Returns a whether the supplied \a filter can be implemented
  natively by this engine.  If not, the base class implementation
  will emulate the functionality.
 */
bool QContactManagerEngine::isFilterSupported(const QContactFilter& filter) const
{
    Q_UNUSED(filter);

    return false;
}

/*!
  Returns the list of data types supported by this engine.
 */
QList<QVariant::Type> QContactManagerEngine::supportedDataTypes() const
{
    return QList<QVariant::Type>();
}

/*!
  Returns true if the manager supports the relationship type specified in \a relationshipType for
  contacts whose type is the given \a contactType.

  Note that some managers may support the relationship type for a contact in a limited manner
  (for example, only as the first contact in the relationship, or only as the second contact
  in the relationship).  In this case, it will still return true.  It will only return false
  if the relationship is entirely unsupported for the given type of contact.
 */
bool QContactManagerEngine::isRelationshipTypeSupported(const QString& relationshipType, const QString& contactType) const
{
    Q_UNUSED(relationshipType);
    Q_UNUSED(contactType);

    return false;
}

/*!
  Returns the list of contact types which are supported by this engine.
  This is a convenience function, equivalent to retrieving the allowable values
  for the \c QContactType::FieldType field of the QContactType definition
  which is valid in this engine.
 */
QStringList QContactManagerEngine::supportedContactTypes() const
{
    QContactManager::Error error;
    QList<QVariant> allowableVals = detailDefinition(QContactType::DefinitionName, QContactType::TypeContact, &error).fields().value(QContactType::FieldType).allowableValues();
    QStringList retn;
    for (int i = 0; i < allowableVals.size(); i++)
        retn += allowableVals.at(i).toString();
    return retn;
}

/*!
  \fn int QContactManagerEngine::managerVersion() const

  Returns the engine backend implementation version number
 */

/*! Returns the base schema definitions */
QMap<QString, QMap<QString, QContactDetailDefinition> > QContactManagerEngine::schemaDefinitions()
{
    // This implementation provides the base schema.
    // The schema documentation (contactsschema.qdoc)
    // MUST BE KEPT UP TO DATE as definitions are added here.

    // the map we will eventually return
    QMap<QString, QContactDetailDefinition> retn;

    // local variables for reuse
    QMap<QString, QContactDetailFieldDefinition> fields;
    QContactDetailFieldDefinition f;
    QContactDetailDefinition d;
    QVariantList contexts;
    contexts << QString(QLatin1String(QContactDetail::ContextHome)) << QString(QLatin1String(QContactDetail::ContextWork)) << QString(QLatin1String(QContactDetail::ContextOther));
    QVariantList subTypes;

    // sync target
    d.setName(QContactSyncTarget::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactSyncTarget::FieldSyncTarget, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(true);
    retn.insert(d.name(), d);

    // timestamp
    d.setName(QContactTimestamp::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::DateTime);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactTimestamp::FieldModificationTimestamp, f);
    fields.insert(QContactTimestamp::FieldCreationTimestamp, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(true);
    retn.insert(d.name(), d);

    // type
    d.setName(QContactType::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    subTypes.clear();
    subTypes << QString(QLatin1String(QContactType::TypeContact)) << QString(QLatin1String(QContactType::TypeGroup));
    f.setAllowableValues(subTypes);
    fields.insert(QContactType::FieldType, f); // note: NO CONTEXT!!
    d.setFields(fields);
    d.setUnique(true);
    retn.insert(d.name(), d);

    // guid
    d.setName(QContactGuid::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactGuid::FieldGuid, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // display label
    d.setName(QContactDisplayLabel::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactDisplayLabel::FieldLabel, f);
    d.setFields(fields);
    d.setUnique(true);
    retn.insert(d.name(), d);

    // email address
    d.setName(QContactEmailAddress::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactEmailAddress::FieldEmailAddress, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // organisation
    d.setName(QContactOrganization::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactOrganization::FieldName, f);
    fields.insert(QContactOrganization::FieldLocation, f);
    fields.insert(QContactOrganization::FieldTitle, f);
    f.setDataType(QVariant::Url);
    fields.insert(QContactOrganization::FieldLogoUrl, f);
    f.setDataType(QVariant::StringList);
    fields.insert(QContactOrganization::FieldDepartment, f);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // phone number
    d.setName(QContactPhoneNumber::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactPhoneNumber::FieldNumber, f);
    f.setDataType(QVariant::StringList); // can implement multiple subtypes
    subTypes.clear();
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeAssistant));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeBulletinBoardSystem));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeCar));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeDtmfMenu));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeFax));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeLandline));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeMessagingCapable));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeMobile));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeModem));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypePager));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeVideo));
    subTypes << QString(QLatin1String(QContactPhoneNumber::SubTypeVoice));
    f.setAllowableValues(subTypes);
    fields.insert(QContactPhoneNumber::FieldSubTypes, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // anniversary
    d.setName(QContactAnniversary::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::Date);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactAnniversary::FieldOriginalDate, f);
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactAnniversary::FieldCalendarId, f);
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactAnniversary::FieldEvent, f);
    f.setDataType(QVariant::String); // only allowed to be a single subtype.
    subTypes.clear();
    subTypes << QString(QLatin1String(QContactAnniversary::SubTypeEmployment));
    subTypes << QString(QLatin1String(QContactAnniversary::SubTypeEngagement));
    subTypes << QString(QLatin1String(QContactAnniversary::SubTypeHouse));
    subTypes << QString(QLatin1String(QContactAnniversary::SubTypeMemorial));
    subTypes << QString(QLatin1String(QContactAnniversary::SubTypeWedding));
    f.setAllowableValues(subTypes);
    fields.insert(QContactAnniversary::FieldSubType, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // birthday
    d.setName(QContactBirthday::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::Date);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactBirthday::FieldBirthday, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(true);
    retn.insert(d.name(), d);

    // nickname
    d.setName(QContactNickname::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactNickname::FieldNickname, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // note
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    d.setName(QContactNote::DefinitionName);
    fields.insert(QContactNote::FieldNote, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // url
    d.setName(QContactUrl::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactUrl::FieldUrl, f);
    f.setDataType(QVariant::String); // only allowed to be a single subtype
    subTypes.clear();
    subTypes << QString(QLatin1String(QContactUrl::SubTypeFavourite));
    subTypes << QString(QLatin1String(QContactUrl::SubTypeHomePage));
    f.setAllowableValues(subTypes);
    fields.insert(QContactUrl::FieldSubType, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // gender
    d.setName(QContactGender::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList() << QString(QLatin1String(QContactGender::GenderMale)) << QString(QLatin1String(QContactGender::GenderFemale)) << QString(QLatin1String(QContactGender::GenderUnspecified)));
    fields.insert(QContactGender::FieldGender, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // online account
    d.setName(QContactOnlineAccount::DefinitionName);
    fields.clear();
    f.setAllowableValues(QVariantList());
    f.setDataType(QVariant::String);
    fields.insert(QContactOnlineAccount::FieldAccountUri, f);
    f.setDataType(QVariant::StringList);
    fields.insert(QContactOnlineAccount::FieldCapabilities, f);
    f.setDataType(QVariant::String);
    fields.insert(QContactOnlineAccount::FieldAccountUri, f);
    fields.insert(QContactOnlineAccount::FieldServiceProvider, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    f.setAllowableValues(QVariantList()); // allow any subtypes!
    fields.insert(QContactOnlineAccount::FieldSubTypes, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // presence
    d.setName(QContactPresence::DefinitionName);
    fields.clear();
    f.setAllowableValues(QVariantList());
    f.setDataType(QVariant::DateTime);
    fields.insert(QContactPresence::FieldTimestamp, f);
    f.setDataType(QVariant::String);
    fields.insert(QContactPresence::FieldNickname, f);
    fields.insert(QContactPresence::FieldCustomMessage, f);
    fields.insert(QContactPresence::FieldPresenceStateText, f);
    QVariantList presenceValues;
    presenceValues << QContactPresence::PresenceAvailable;
    presenceValues << QContactPresence::PresenceAway;
    presenceValues << QContactPresence::PresenceBusy;
    presenceValues << QContactPresence::PresenceExtendedAway;
    presenceValues << QContactPresence::PresenceHidden;
    presenceValues << QContactPresence::PresenceOffline;
    presenceValues << QContactPresence::PresenceUnknown;
    f.setAllowableValues(presenceValues);
    f.setDataType(QVariant::Int);
    fields.insert(QContactPresence::FieldPresenceState, f);
    f.setAllowableValues(QVariantList());
    f.setDataType(QVariant::Url);
    fields.insert(QContactPresence::FieldPresenceStateImageUrl, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // global presence
    d.setName(QContactGlobalPresence::DefinitionName);
    fields.clear();
    f.setAllowableValues(QVariantList());
    f.setDataType(QVariant::DateTime);
    fields.insert(QContactGlobalPresence::FieldTimestamp, f);
    f.setDataType(QVariant::String);
    fields.insert(QContactGlobalPresence::FieldNickname, f);
    fields.insert(QContactGlobalPresence::FieldCustomMessage, f);
    fields.insert(QContactGlobalPresence::FieldPresenceStateText, f);
    f.setAllowableValues(presenceValues);
    f.setDataType(QVariant::Int);
    fields.insert(QContactGlobalPresence::FieldPresenceState, f);
    f.setAllowableValues(QVariantList());
    f.setDataType(QVariant::Url);
    fields.insert(QContactGlobalPresence::FieldPresenceStateImageUrl, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(true); // unique and read only!
    retn.insert(d.name(), d);

    // avatar
    d.setName(QContactAvatar::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::Url);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactAvatar::FieldImageUrl, f);
    fields.insert(QContactAvatar::FieldVideoUrl, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // ringtone
    d.setName(QContactRingtone::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::Url);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactRingtone::FieldAudioRingtoneUrl, f);
    fields.insert(QContactRingtone::FieldVideoRingtoneUrl, f);
    fields.insert(QContactRingtone::FieldVibrationRingtoneUrl, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // thumbnail
    d.setName(QContactThumbnail::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::Image);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactThumbnail::FieldThumbnail, f);
    d.setFields(fields);
    d.setUnique(true); // only one thumbnail, no context.
    retn.insert(d.name(), d);

    // GeoLocation
    d.setName(QContactGeoLocation::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactGeoLocation::FieldLabel, f);
    f.setDataType(QVariant::Double);
    fields.insert(QContactGeoLocation::FieldLatitude, f);
    fields.insert(QContactGeoLocation::FieldLongitude, f);
    fields.insert(QContactGeoLocation::FieldAccuracy, f);
    fields.insert(QContactGeoLocation::FieldAltitude, f);
    fields.insert(QContactGeoLocation::FieldAltitudeAccuracy, f);
    fields.insert(QContactGeoLocation::FieldSpeed, f);
    fields.insert(QContactGeoLocation::FieldHeading, f);
    f.setDataType(QVariant::DateTime);
    fields.insert(QContactGeoLocation::FieldTimestamp, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // street address
    d.setName(QContactAddress::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactAddress::FieldPostOfficeBox, f);
    fields.insert(QContactAddress::FieldStreet, f);
    fields.insert(QContactAddress::FieldLocality, f);
    fields.insert(QContactAddress::FieldRegion, f);
    fields.insert(QContactAddress::FieldPostcode, f);
    fields.insert(QContactAddress::FieldCountry, f);
    f.setDataType(QVariant::StringList); // can implement multiple subtypes
    subTypes.clear();
    subTypes << QString(QLatin1String(QContactAddress::SubTypeDomestic));
    subTypes << QString(QLatin1String(QContactAddress::SubTypeInternational));
    subTypes << QString(QLatin1String(QContactAddress::SubTypeParcel));
    subTypes << QString(QLatin1String(QContactAddress::SubTypePostal));
    f.setAllowableValues(subTypes);
    fields.insert(QContactAddress::FieldSubTypes, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // name
    d.setName(QContactName::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactName::FieldPrefix, f);
    fields.insert(QContactName::FieldFirstName, f);
    fields.insert(QContactName::FieldMiddleName, f);
    fields.insert(QContactName::FieldLastName, f);
    fields.insert(QContactName::FieldSuffix, f);
    fields.insert(QContactName::FieldCustomLabel, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // tag
    d.setName(QContactTag::DefinitionName);
    fields.clear();
    f.setDataType(QVariant::String);
    f.setAllowableValues(QVariantList());
    fields.insert(QContactTag::FieldTag, f);
    f.setDataType(QVariant::StringList);
    f.setAllowableValues(contexts);
    fields.insert(QContactDetail::FieldContext, f);
    d.setFields(fields);
    d.setUnique(false);
    retn.insert(d.name(), d);

    // in the default schema, we have two contact types: TypeContact, TypeGroup.
    // the entire default schema is valid for both types.
    QMap<QString, QMap<QString, QContactDetailDefinition> > retnSchema;
    retnSchema.insert(QContactType::TypeContact, retn);
    retnSchema.insert(QContactType::TypeGroup, retn);

    return retnSchema;
}

/*!
  Checks that the given contact \a contact does not have details which
  don't conform to a valid definition, violate uniqueness constraints,
  or contain values for nonexistent fields, and that the values contained are
  of the correct type for each field, and are allowable values for that field.

  Note that this function is unable to ensure that the access constraints
  (such as CreateOnly and ReadOnly) are observed; backend specific code
  must be written if you wish to enforce these constraints.

  Returns true if the \a contact is valid according to the definitions for
  its details, otherwise returns false.

  Any errors encountered during this operation should be stored to
  \a error.
 */
bool QContactManagerEngine::validateContact(const QContact& contact, QContactManager::Error* error) const
{
    QList<QString> uniqueDefinitionIds;

    // check that each detail conforms to its definition as supported by this manager.
    foreach (const QContactDetail& detail, contact.details()) {
        QVariantMap values = detail.variantValues();
        QContactDetailDefinition def = detailDefinition(detail.definitionName(), contact.type(), error);
        // check that the definition is supported
        if (*error != QContactManager::NoError) {
            *error = QContactManager::InvalidDetailError;
            return false; // this definition is not supported.
        }

        // check uniqueness
        if (def.isUnique()) {
            if (uniqueDefinitionIds.contains(def.name())) {
                *error = QContactManager::AlreadyExistsError;
                return false; // can't have two of a unique detail.
            }
            uniqueDefinitionIds.append(def.name());
        }

        QMapIterator<QString,QVariant> fieldIt(values);
        while (fieldIt.hasNext()) {
            fieldIt.next();
            const QString& key = fieldIt.key();
            const QVariant& variant = fieldIt.value();
            // check that no values exist for nonexistent fields.
            if (!def.fields().contains(key)) {
                *error = QContactManager::InvalidDetailError;
                return false; // value for nonexistent field.
            }

            QContactDetailFieldDefinition field = def.fields().value(key);
            // check that the type of each value corresponds to the allowable field type
            if (static_cast<int>(field.dataType()) != variant.userType()) {
                *error = QContactManager::InvalidDetailError;
                return false; // type doesn't match.
            }

            // check that the value is allowable
            // if the allowable values is an empty list, any are allowed.
            if (!field.allowableValues().isEmpty()) {
                // if the field datatype is a list, check that it contains only allowable values
                if (field.dataType() == QVariant::List || field.dataType() == QVariant::StringList) {
                    QList<QVariant> innerValues = variant.toList();
                    QListIterator<QVariant> it(innerValues);
                    while (it.hasNext()) {
                        if (!field.allowableValues().contains(it.next())) {
                            *error = QContactManager::InvalidDetailError;
                            return false; // value not allowed.
                        }
                    }
                } else if (!field.allowableValues().contains(variant)) {
                    // the datatype is not a list; the value wasn't allowed.
                    *error = QContactManager::InvalidDetailError;
                    return false; // value not allowed.
                }
            }
        }
    }

    return true;
}

/*!
  Checks that the given detail definition \a definition seems valid,
  with a correct id, defined fields, and any specified value types
  are supported by this engine.  This function is called before
  trying to save a definition.

  Returns true if the \a definition seems valid, otherwise returns
  false.

  Any errors encountered during this operation should be stored to
  \a error.
 */
bool QContactManagerEngine::validateDefinition(const QContactDetailDefinition& definition, QContactManager::Error* error) const
{
    if (definition.name().isEmpty()) {
        *error = QContactManager::BadArgumentError;
        return false;
    }

    if (definition.fields().count() == 0) {
        *error = QContactManager::BadArgumentError;
        return false;
    }

    // Check each field now
    QList<QVariant::Type> types = supportedDataTypes();
    QMapIterator<QString, QContactDetailFieldDefinition> it(definition.fields());
    while(it.hasNext()) {
        it.next();
        if (it.key().isEmpty()) {
            *error = QContactManager::BadArgumentError;
            return false;
        }

        if (!types.contains(it.value().dataType())) {
            *error = QContactManager::BadArgumentError;
            return false;
        }

        // Check that each allowed value is the same type
        for (int i=0; i < it.value().allowableValues().count(); i++) {
            if (it.value().allowableValues().at(i).type() != it.value().dataType()) {
                *error = QContactManager::BadArgumentError;
                return false;
            }
        }
    }
    *error = QContactManager::NoError;
    return true;
}

/*!
  Returns the registered detail definitions which are valid for contacts whose type is of the given \a contactType in this engine.

  Any errors encountered during this operation should be stored to
  \a error.
 */
QMap<QString, QContactDetailDefinition> QContactManagerEngine::detailDefinitions(const QString& contactType, QContactManager::Error* error) const
{
    Q_UNUSED(contactType);
    *error = QContactManager::NotSupportedError;
    return QMap<QString, QContactDetailDefinition>();
}

/*!
  Returns the definition identified by the given \a definitionName that
  is valid for contacts whose type is of the given \a contactType in this store, or a default-constructed QContactDetailDefinition
  if no such definition exists

  Any errors encountered during this operation should be stored to
  \a error.
 */
QContactDetailDefinition QContactManagerEngine::detailDefinition(const QString& definitionName, const QString& contactType, QContactManager::Error* error) const
{
    QMap<QString, QContactDetailDefinition> definitions = detailDefinitions(contactType, error);
    if (definitions.contains(definitionName))  {
        *error = QContactManager::NoError;
        return definitions.value(definitionName);
    } else {
        *error = QContactManager::DoesNotExistError;
        return QContactDetailDefinition();
    }
}

/*!
  Persists the given definition \a def in the database, which is valid for contacts whose type is the given \a contactType.

  Returns true if the definition was saved successfully, and otherwise returns false.

  The backend must emit the appropriate signals to inform clients of changes
  to the database resulting from this operation.

  Any errors encountered during this operation should be stored to
  \a error.
 */
bool QContactManagerEngine::saveDetailDefinition(const QContactDetailDefinition& def, const QString& contactType, QContactManager::Error* error)
{
    Q_UNUSED(def);
    Q_UNUSED(contactType);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Removes the definition identified by the given \a definitionName from the database, where it was valid for contacts whose type was the given \a contactType.

  Returns true if the definition was removed successfully, otherwise returns false.

  The backend must emit the appropriate signals to inform clients of changes
  to the database resulting from this operation.

  Any errors encountered during this operation should be stored to
  \a error.
 */
bool QContactManagerEngine::removeDetailDefinition(const QString& definitionName, const QString& contactType, QContactManager::Error* error)
{
    Q_UNUSED(definitionName);
    Q_UNUSED(contactType);

    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Sets the access constraints of \a detail to the supplied \a constraints.

  This function is provided to allow engine implementations to report the
  access constraints of retrieved details, without generally allowing the
  access constraints to be modified after retrieval.

  Application code should not call this function, since validation of the
  detail will happen in the engine in any case.
 */
void QContactManagerEngine::setDetailAccessConstraints(QContactDetail *detail, QContactDetail::AccessConstraints constraints)
{
    if (detail) {
        QContactDetailPrivate::setAccessConstraints(detail, constraints);
    }
}


/*!
  Adds the given \a contact to the database if \a contact has a
  default-constructed id, or an id with the manager URI set to the URI of
  this manager and a local id of zero, otherwise updates the contact in
  the database which has the same id to be the given \a contact.
  If the id is non-zero but does not identify any contact stored in the
  manager, the function will return false and \a error will be set to
  \c QContactManager::DoesNotExistError.

  Returns true if the save operation completed successfully, otherwise
  returns false.  Any error which occurs will be saved in \a error.

  The default implementation will convert this into a call to saveContacts.

  \sa managerUri()
 */
bool QContactManagerEngine::saveContact(QContact* contact, QContactManager::Error* error)
{
    // Convert to a list op
    if (contact) {
        QList<QContact> list;
        list.append(*contact);

        QMap<int, QContactManager::Error> errors;
        bool ret = saveContacts(&list, &errors, error);

        if (errors.count() > 0)
            *error = errors.begin().value();

        *contact = list.value(0);
        return ret;
    } else {
        *error = QContactManager::BadArgumentError;
        return false;
    }
}

/*!
  Remove the contact identified by \a contactId from the database,
  and also removes any relationships in which the contact was involved.
  Returns true if the contact was removed successfully, otherwise
  returns false.

  Any error which occurs will be saved in \a error.

  The default implementation will convert this into a call to removeContacts.
 */
bool QContactManagerEngine::removeContact(const QContactLocalId& contactId, QContactManager::Error* error)
{
    // Convert to a list op
    QList<QContactLocalId> list;
    list.append(contactId);

    QMap<int, QContactManager::Error> errors;
    bool ret = removeContacts(list, &errors, error);

    if (errors.count() > 0)
        *error = errors.begin().value();

    return ret;
}

/*!
  Adds the list of contacts given by \a contacts list to the database.
  Returns true if the contacts were saved successfully, otherwise false.

  The manager might populate \a errorMap (the map of indices of the \a contacts list to
  the error which occurred when saving the contact at that index) for
  every index for which the contact could not be saved, if it is able.
  The \l QContactManager::error() function will only return \c QContactManager::NoError
  if all contacts were saved successfully.

  For each newly saved contact that was successful, the id of the contact
  in the \a contacts list will be updated with the new value.  If a failure occurs
  when saving a new contact, the id will be cleared.

  Any errors encountered during this operation should be stored to
  \a error.

  \sa QContactManager::saveContact()
 */
bool QContactManagerEngine::saveContacts(QList<QContact>* contacts, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(contacts);
    Q_UNUSED(errorMap);
    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Remove every contact whose id is contained in the list of contacts ids
  \a contactIds.  Returns true if all contacts were removed successfully,
  otherwise false.

  Any contact that was removed successfully will have the relationships
  in which it was involved removed also.

  The manager might populate \a errorMap (the map of indices of the \a contactIds list to
  the error which occurred when saving the contact at that index) for every
  index for which the contact could not be removed, if it is able.
  The \l QContactManager::error() function will
  only return \c QContactManager::NoError if all contacts were removed
  successfully.

  If the list contains ids which do not identify a valid contact in the manager, the function will
  remove any contacts which are identified by ids in the \a contactIds list, insert
  \c QContactManager::DoesNotExist entries into the \a errorMap for the indices of invalid ids
  in the \a contactIds list, return false, and set the overall operation error to
  \c QContactManager::DoesNotExistError.

  Any errors encountered during this operation should be stored to
  \a error.

  \sa QContactManager::removeContact()
 */
bool QContactManagerEngine::removeContacts(const QList<QContactLocalId>& contactIds, QMap<int, QContactManager::Error>* errorMap, QContactManager::Error* error)
{
    Q_UNUSED(contactIds);
    Q_UNUSED(errorMap);
    *error = QContactManager::NotSupportedError;
    return false;
}

/*!
  Returns a pruned or modified version of the \a original contact which is valid and can be saved in the manager.
  The returned contact might have details removed or arbitrarily changed.  The cache of relationships
  in the contact are ignored entirely when considering compatibility with the backend, as they are
  saved and validated separately.  Any error which occurs will be saved to \a error.
 */
QContact QContactManagerEngine::compatibleContact(const QContact& original, QContactManager::Error* error) const
{
    QContact conforming;
    conforming.setId(original.id());
    QContactManager::Error tempError;
    QList<QString> uniqueDefinitionIds;
    foreach (QContactDetail detail, original.details()) {
        // check that the detail conforms to the definition in this manager.
        // if so, then add it to the conforming contact to be returned.  if not, prune it.

        QVariantMap values = detail.variantValues();
        QContactDetailDefinition def = detailDefinition(detail.definitionName(), original.type(), &tempError);
        // check that the definition is supported
        if (tempError != QContactManager::NoError) {
            continue; // this definition is not supported.
        }

        // check uniqueness
        if (def.isUnique()) {
            if (uniqueDefinitionIds.contains(def.name())) {
                continue; // can't have two of a unique detail.
            }
            uniqueDefinitionIds.append(def.name());
        }

        QMapIterator<QString,QVariant> fieldIt(values);
        while (fieldIt.hasNext()) {
            fieldIt.next();
            const QString& key = fieldIt.key();
            const QVariant& variant = fieldIt.value();
            // prune values for nonexistent fields.
            if (!def.fields().contains(key)) {
                detail.removeValue(key);
            }

            QContactDetailFieldDefinition field = def.fields().value(key);
            // prune values that do not correspond to the allowable field type
            if (static_cast<int>(field.dataType()) != variant.userType()) {
                detail.removeValue(key);
            }

            // check that the value is allowable
            // if the allowable values is an empty list, any are allowed.
            if (!field.allowableValues().isEmpty()) {
                // if the field datatype is a list, remove non-allowable values
                if (field.dataType() == QVariant::List || field.dataType() == QVariant::StringList) {
                    QList<QVariant> innerValues = variant.toList();
                    QMutableListIterator<QVariant> it(innerValues);
                    while (it.hasNext()) {
                        if (!field.allowableValues().contains(it.next())) {
                            it.remove();
                        }
                    }
                    if (innerValues.isEmpty())
                        detail.removeValue(key);
                    else
                        detail.setValue(key, innerValues);
                } else if (!field.allowableValues().contains(variant)) {
                    detail.removeValue(key);
                }
            }
        }

        // if it hasn't been pruned away to nothing, save it in the conforming contact
        if (!detail.isEmpty()) {
            conforming.saveDetail(&detail);
        }
    }

    if (!conforming.isEmpty())
        *error = QContactManager::NoError;
    else
        *error = QContactManager::DoesNotExistError;
    return conforming;
}

/*!
  Compares \a first against \a second.  If the types are
  strings (QVariant::String), the \a sensitivity argument controls
  case sensitivity when comparing.

  Returns:
  <0 if \a first is less than \a second
   0 if \a first is equal to \a second
  >0 if \a first is greater than \a second.

  The results are undefined if the variants are different types, or
  cannot be compared.
 */
int QContactManagerEngine::compareVariant(const QVariant& first, const QVariant& second, Qt::CaseSensitivity sensitivity)
{
    switch(first.type()) {
        case QVariant::Int:
            return first.toInt() - second.toInt();

        case QVariant::LongLong:
            return first.toLongLong() - second.toLongLong();

        case QVariant::Bool:
        case QVariant::Char:
        case QVariant::UInt:
            return first.toUInt() - second.toUInt();

        case QVariant::ULongLong:
            return first.toULongLong() - second.toULongLong();

       case QVariant::String:
            return first.toString().compare(second.toString(), sensitivity);

        case QVariant::Double:
            {
                const double a = first.toDouble();
                const double b = second.toDouble();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::DateTime:
            {
                const QDateTime a = first.toDateTime();
                const QDateTime b = second.toDateTime();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        case QVariant::Date:
            return first.toDate().toJulianDay() - second.toDate().toJulianDay();

        case QVariant::Time:
            {
                const QTime a = first.toTime();
                const QTime b = second.toTime();
                return (a < b) ? -1 : ((a == b) ? 0 : 1);
            }

        default:
            return 0;
    }
}

/*!
  Returns true if the supplied contact \a contact matches the supplied filter \a filter.

  This function will test each condition in the filter, possibly recursing.
 */
bool QContactManagerEngine::testFilter(const QContactFilter &filter, const QContact &contact)
{
    switch(filter.type()) {
        case QContactFilter::InvalidFilter:
        case QContactFilter::ActionFilter:
            return false;

        case QContactFilter::DefaultFilter:
            return true;

        case QContactFilter::LocalIdFilter:
            {
                const QContactLocalIdFilter idf(filter);
                if (idf.ids().contains(contact.id().localId()))
                    return true;
            }
            // Fall through to end
            break;

        case QContactFilter::ContactDetailFilter:
            {
                const QContactDetailFilter cdf(filter);
                if (cdf.detailDefinitionName().isEmpty())
                    return false;

                /* See if this contact has one of these details in it */
                const QList<QContactDetail>& details = contact.details(cdf.detailDefinitionName());

                if (details.count() == 0)
                    return false; /* can't match */

                /* See if we need to check the values */
                if (cdf.detailFieldName().isEmpty())
                    return true;  /* just testing for the presence of a detail of the specified definition */

                /* Now figure out what tests we are doing */
                const bool valueTest = cdf.value().isValid();
                const bool presenceTest = !valueTest;

                /* See if we need to test any values at all */
                if (presenceTest) {
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);

                        /* Check that the field is present and has a non-empty value */
                        if (detail.variantValues().contains(cdf.detailFieldName()) && !detail.value(cdf.detailFieldName()).isEmpty())
                            return true;
                    }
                    return false;
                }

                /* Case sensitivity, for those parts that use it */
                Qt::CaseSensitivity cs = (cdf.matchFlags() & QContactFilter::MatchCaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive;

                /* See what flags are requested, since we're looking at a value */
                if (cdf.matchFlags() & QContactFilter::MatchPhoneNumber) {
                    /* Doing phone number filtering.  We hand roll an implementation here, backends will obviously want to override this. */
                    QString input = cdf.value().toString();

                    /* preprocess the input - ignore any non-digits (doesn't perform ITU-T collation */
                    QString preprocessedInput;
                    for (int i = 0; i < input.size(); i++) {
                        QChar current = input.at(i).toLower();
                        if (current.isDigit()) preprocessedInput.append(current);
                        // note: we ignore characters like '+', 'p', 'w', '*' and '#' which may be important.
                    }

                    /* Look at every detail in the set of details and compare */
                    for (int j = 0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QString& valueString = detail.value(cdf.detailFieldName());
                        QString preprocessedValueString;
                        for (int i = 0; i < valueString.size(); i++) {
                            QChar current = valueString.at(i).toLower();
                            if (current.isDigit()) preprocessedValueString.append(current);
                            // note: we ignore characters like '+', 'p', 'w', '*' and '#' which may be important.
                        }

                        // if the matchflags input don't require a particular criteria to pass, we assume that it has passed.
                        // the "default" match strategy is an "endsWith" strategy.
                        bool me = (cdf.matchFlags() & 7) == QContactFilter::MatchExactly;
                        bool mc = (cdf.matchFlags() & 7) == QContactFilter::MatchContains;
                        bool msw = (cdf.matchFlags() & 7) == QContactFilter::MatchStartsWith;
                        bool mew = (cdf.matchFlags() & 7) == QContactFilter::MatchEndsWith;

                        bool mer = (me ? preprocessedValueString == preprocessedInput : true);
                        bool mcr = (mc ? preprocessedValueString.contains(preprocessedInput) : true);
                        bool mswr = (msw ? preprocessedValueString.startsWith(preprocessedInput) : true);
                        bool mewr = (mew ? preprocessedValueString.endsWith(preprocessedInput) : true);
                        if (mewr && mswr && mcr && mer) {
                            return true; // this detail meets all of the criteria which were required, and hence must match.
                        }
                    }
                } else if (cdf.matchFlags() & QContactFilter::MatchKeypadCollation) {
                    // XXX TODO: not sure about the filtering semantics for MatchKeypadCollation.
                    QString input = cdf.value().toString();

                    /* Look at every detail in the set of details and compare */
                    for (int j = 0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QString& valueString = detail.value(cdf.detailFieldName()).toLower();

                        // preprocess the valueString
                        QString preprocessedValue;
                        for (int i = 0; i < valueString.size(); i++) {
                            // we use ITU-T keypad collation by default.
                            QChar currentValueChar = valueString.at(i);
                            if (currentValueChar == QLatin1Char('a') || currentValueChar == QLatin1Char('b') || currentValueChar == QLatin1Char('c'))
                                preprocessedValue.append(QLatin1Char('2'));
                            else if (currentValueChar == QLatin1Char('d') || currentValueChar == QLatin1Char('e') || currentValueChar == QLatin1Char('f'))
                                preprocessedValue.append(QLatin1Char('3'));
                            else if (currentValueChar == QLatin1Char('g') || currentValueChar == QLatin1Char('h') || currentValueChar == QLatin1Char('i'))
                                preprocessedValue.append(QLatin1Char('4'));
                            else if (currentValueChar == QLatin1Char('j') || currentValueChar == QLatin1Char('k') || currentValueChar == QLatin1Char('l'))
                                preprocessedValue.append(QLatin1Char('5'));
                            else if (currentValueChar == QLatin1Char('m') || currentValueChar == QLatin1Char('n') || currentValueChar == QLatin1Char('o'))
                                preprocessedValue.append(QLatin1Char('6'));
                            else if (currentValueChar == QLatin1Char('p') || currentValueChar == QLatin1Char('q') || currentValueChar == QLatin1Char('r') || currentValueChar == QLatin1Char('s'))
                                preprocessedValue.append(QLatin1Char('7'));
                            else if (currentValueChar == QLatin1Char('t') || currentValueChar == QLatin1Char('u') || currentValueChar == QLatin1Char('v'))
                                preprocessedValue.append(QLatin1Char('8'));
                            else if (currentValueChar == QLatin1Char('w') || currentValueChar == QLatin1Char('x') || currentValueChar == QLatin1Char('y') || currentValueChar == QLatin1Char('z'))
                                preprocessedValue.append(QLatin1Char('9'));
                            else
                                preprocessedValue.append(currentValueChar);
                        }

                        bool me = (cdf.matchFlags() & 7) == QContactFilter::MatchExactly;
                        bool mc = (cdf.matchFlags() & 7) == QContactFilter::MatchContains;
                        bool msw = (cdf.matchFlags() & 7) == QContactFilter::MatchStartsWith;
                        bool mew = (cdf.matchFlags() & 7) == QContactFilter::MatchEndsWith;

                        bool mer = (me ? preprocessedValue == input : true);
                        bool mcr = (mc ? preprocessedValue.contains(input) : true);
                        bool mswr = (msw ? preprocessedValue.startsWith(input) : true);
                        bool mewr = (mew ? preprocessedValue.endsWith(input) : true);
                        if (mewr && mswr && mcr && mer) {
                            return true; // this detail meets all of the criteria which were required, and hence must match.
                        }
                    }
                } else if (cdf.matchFlags() & (QContactFilter::MatchEndsWith | QContactFilter::MatchStartsWith | QContactFilter::MatchContains | QContactFilter::MatchFixedString)) {
                    /* We're strictly doing string comparisons here */
                    bool matchStarts = (cdf.matchFlags() & 7) == QContactFilter::MatchStartsWith;
                    bool matchEnds = (cdf.matchFlags() & 7) == QContactFilter::MatchEndsWith;
                    bool matchContains = (cdf.matchFlags() & 7) == QContactFilter::MatchContains;

                    /* Value equality test */
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QString& var = detail.value(cdf.detailFieldName());
                        const QString& needle = cdf.value().toString();
                        if (matchStarts && var.startsWith(needle, cs))
                            return true;
                        if (matchEnds && var.endsWith(needle, cs))
                            return true;
                        if (matchContains && var.contains(needle, cs))
                            return true;
                        if (QString::compare(var, needle, cs) == 0)
                            return true;
                    }
                    return false;
                } else {
                    /* Nope, testing the values as a variant */
                    /* Value equality test */
                    for(int j = 0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QVariant& var = detail.variantValue(cdf.detailFieldName());
                        if (!var.isNull() && compareVariant(var, cdf.value(), cs) == 0)
                            return true;
                    }
                }
            }
            break;

        case QContactFilter::ContactDetailRangeFilter:
            {
                const QContactDetailRangeFilter cdf(filter);
                if (cdf.detailDefinitionName().isEmpty())
                    return false; /* we do not know which field to check */

                /* See if this contact has one of these details in it */
                const QList<QContactDetail>& details = contact.details(cdf.detailDefinitionName());

                if (details.count() == 0)
                    return false; /* can't match */

                /* Check for a detail presence test */
                if (cdf.detailFieldName().isEmpty())
                    return true;

                /* See if this is a field presence test */
                if (!cdf.minValue().isValid() && !cdf.maxValue().isValid()) {
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        if (detail.variantValues().contains(cdf.detailFieldName()))
                            return true;
                    }
                    return false;
                }

                /* open or closed interval testing support */
                const int minComp = cdf.rangeFlags() & QContactDetailRangeFilter::ExcludeLower ? 1 : 0;
                const int maxComp = cdf.rangeFlags() & QContactDetailRangeFilter::IncludeUpper ? 1 : 0;

                const bool testMin = cdf.minValue().isValid();
                const bool testMax = cdf.maxValue().isValid();

                /* At this point we know that at least of testMin & testMax is true */

                /* Case sensitivity, for those parts that use it */
                Qt::CaseSensitivity cs = (cdf.matchFlags() & QContactFilter::MatchCaseSensitive) ? Qt::CaseSensitive : Qt::CaseInsensitive;

                /* See what flags are requested, since we're looking at a value */
                if (cdf.matchFlags() & (QContactFilter::MatchEndsWith | QContactFilter::MatchStartsWith | QContactFilter::MatchContains | QContactFilter::MatchFixedString)) {
                    /* We're strictly doing string comparisons here */
                    //bool matchStarts = (cdf.matchFlags() & 7) == QContactFilter::MatchStartsWith;
                    bool matchEnds = (cdf.matchFlags() & 7) == QContactFilter::MatchEndsWith;
                    bool matchContains = (cdf.matchFlags() & 7) == QContactFilter::MatchContains;

                    /* Min/Max and contains do not make sense */
                    if (matchContains)
                        return false;

                    QString minVal = cdf.minValue().toString();
                    QString maxVal = cdf.maxValue().toString();

                    /* Starts with is the normal compare case, endsWith is a bit trickier */
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QString& var = detail.value(cdf.detailFieldName());
                        if (!matchEnds) {
                            // MatchStarts or MatchFixedString
                            if (testMin && QString::compare(var, minVal, cs) < minComp)
                                continue;
                            if (testMax && QString::compare(var, maxVal, cs) >= maxComp)
                                continue;
                            return true;
                        } else {
                            /* Have to test the length of min & max */
                            // using refs means the parameter order is backwards, so negate the result of compare
                            if (testMin && -QString::compare(minVal, var.rightRef(minVal.length()), cs) < minComp)
                                continue;
                            if (testMax && -QString::compare(maxVal, var.rightRef(maxVal.length()), cs) >= maxComp)
                                continue;
                            return true;
                        }
                    }
                    // Fall through to end
                } else {
                    /* Nope, testing the values as a variant */
                    for(int j=0; j < details.count(); j++) {
                        const QContactDetail& detail = details.at(j);
                        const QVariant& var = detail.variantValue(cdf.detailFieldName());

                        if (testMin && compareVariant(var, cdf.minValue(), cs) < minComp)
                            continue;
                        if (testMax && compareVariant(var, cdf.maxValue(), cs) >= maxComp)
                            continue;
                        return true;
                    }
                    // Fall through to end
                }
            }
            break;

        case QContactFilter::RelationshipFilter:
            {
                // matches any contact that plays the specified role in a relationship
                // of the specified type with the specified other participant.
                const QContactRelationshipFilter rf(filter);

                // first, retrieve contact uris
                QContactId contactUri = contact.id();
                QContactId relatedContactId = rf.relatedContactId();

                // get the relationships in which this contact is involved.
                QList<QContactRelationship> allRelationships;
                allRelationships = contact.relationships();

// simplify the comparison of two contact id's depending on their fields.
#define CONTACT_IDS_MATCH(specific, other) \
            ((specific == QContactId()) \
                || (specific.managerUri().isEmpty() && specific.localId() == other.localId()) \
                || (specific.managerUri() == other.managerUri() && specific.localId() == QContactLocalId(0)) \
                || (specific.managerUri() == other.managerUri() && specific.localId() == other.localId()))

                // now check to see if we have a match.
                foreach (const QContactRelationship& rel, allRelationships) {
                    // perform the matching.
                    if (rf.relatedContactRole() == QContactRelationship::Second) { // this is the role of the related contact; ie, to match, contact.id() must be the first in the relationship.
                        if ((rf.relationshipType().isEmpty() || rel.relationshipType() == rf.relationshipType())
                                && CONTACT_IDS_MATCH(rel.first(), contact.id()) && CONTACT_IDS_MATCH(relatedContactId, rel.second())) {
                            return true;
                        }
                    } else if (rf.relatedContactRole() == QContactRelationship::First) { // this is the role of the related contact; ie, to match, contact.id() must be the second in the relationship.
                        if ((rf.relationshipType().isEmpty() || rel.relationshipType() == rf.relationshipType())
                                && CONTACT_IDS_MATCH(rel.second(), contact.id()) && CONTACT_IDS_MATCH(relatedContactId, rel.first())) {
                            return true;
                        }
                    } else { // QContactRelationship::Either
                        if ((rf.relationshipType().isEmpty() || rel.relationshipType() == rf.relationshipType())
                                && ((CONTACT_IDS_MATCH(relatedContactId, rel.first()) && !CONTACT_IDS_MATCH(contactUri, relatedContactId)) || (CONTACT_IDS_MATCH(relatedContactId, rel.second()) && !CONTACT_IDS_MATCH(contactUri, relatedContactId)))) {
                            return true;
                        }
                    }
                }

                // if not found by now, it doesn't match the filter.
                return false;
            }
            //break; // unreachable.

        case QContactFilter::ChangeLogFilter:
            {
                QContactChangeLogFilter ccf(filter);

                // See what we can do...
                QContactTimestamp ts = contact.detail(QContactTimestamp::DefinitionName);

                // See if timestamps are even supported
                if (ts.isEmpty())
                    break;

                if (ccf.eventType() == QContactChangeLogFilter::EventAdded)
                    return ccf.since() <= ts.created();
                if (ccf.eventType() == QContactChangeLogFilter::EventChanged)
                    return ccf.since() <= ts.lastModified();

                // You can't emulate a removed..
                // Fall through to end
            }
            break;

        case QContactFilter::IntersectionFilter:
            {
                /* XXX In theory we could reorder the terms to put the native tests first */
                const QContactIntersectionFilter bf(filter);
                const QList<QContactFilter>& terms = bf.filters();
                if (terms.count() > 0) {
                    for(int j = 0; j < terms.count(); j++) {
                        if (!testFilter(terms.at(j), contact)) {
                            return false;
                        }
                    }
                    return true;
                }
                // Fall through to end
            }
            break;

        case QContactFilter::UnionFilter:
            {
                /* XXX In theory we could reorder the terms to put the native tests first */
                const QContactUnionFilter bf(filter);
                const QList<QContactFilter>& terms = bf.filters();
                if (terms.count() > 0) {
                    for(int j = 0; j < terms.count(); j++) {
                        if (testFilter(terms.at(j), contact)) {
                            return true;
                        }
                    }
                    return false;
                }
                // Fall through to end
            }
            break;
    }
    return false;
}

/*!
  Sets the cached relationships in the given \a contact to \a relationships
 */
void QContactManagerEngine::setContactRelationships(QContact* contact, const QList<QContactRelationship>& relationships)
{
    contact->d->m_relationshipsCache = relationships;
}


/*!
  Compares two contacts (\a a and \a b) using the given list of \a sortOrders.  Returns a negative number if \a a should appear
  before \a b according to the sort order, a positive number if \a a should appear after \a b according to the sort order,
  and zero if the two are unable to be sorted.
 */
int QContactManagerEngine::compareContact(const QContact& a, const QContact& b, const QList<QContactSortOrder>& sortOrders)
{
    QList<QContactSortOrder> copy = sortOrders;
    while (copy.size()) {
        // retrieve the next sort order in the list
        QContactSortOrder sortOrder = copy.takeFirst();
        if (!sortOrder.isValid())
            break;

        // obtain the values which this sort order concerns
        const QVariant& aVal = a.detail(sortOrder.detailDefinitionName()).variantValue(sortOrder.detailFieldName());
        const QVariant& bVal = b.detail(sortOrder.detailDefinitionName()).variantValue(sortOrder.detailFieldName());

        bool aIsNull = false;
        bool bIsNull = false;

        // treat empty strings as null qvariants.
        if ((aVal.type() == QVariant::String && aVal.toString().isEmpty()) || aVal.isNull()) {
            aIsNull = true;
        }
        if ((bVal.type() == QVariant::String && bVal.toString().isEmpty()) || bVal.isNull()) {
            bIsNull = true;
        }

        // early exit error checking
        if (aIsNull && bIsNull)
            continue; // use next sort criteria.
        if (aIsNull)
            return (sortOrder.blankPolicy() == QContactSortOrder::BlanksFirst ? -1 : 1);
        if (bIsNull)
            return (sortOrder.blankPolicy() == QContactSortOrder::BlanksFirst ? 1 : -1);

        // real comparison
        int comparison = compareVariant(aVal, bVal, sortOrder.caseSensitivity()) * (sortOrder.direction() == Qt::AscendingOrder ? 1 : -1);
        if (comparison == 0)
            continue;
        return comparison;
    }

    return 0; // or according to id? return (a.id() < b.id() ? -1 : 1);
}


/*!
  Performs insertion sort of the contact \a toAdd into the \a sorted list, according to the provided \a sortOrders list.
  The first QContactSortOrder in the list has the highest priority; if the contact \a toAdd is deemed equal to another
  in the \a sorted list, the second QContactSortOrder in the list is used (and so on until either the contact is inserted
  or there are no more sort order objects in the list).
 */
void QContactManagerEngine::addSorted(QList<QContact>* sorted, const QContact& toAdd, const QList<QContactSortOrder>& sortOrders)
{
    if (sortOrders.count() > 0) {
        for (int i = 0; i < sorted->size(); i++) {
            // check to see if the new contact should be inserted here
            int comparison = compareContact(sorted->at(i), toAdd, sortOrders);
            if (comparison > 0) {
                sorted->insert(i, toAdd);
                return;
            }
        }
    }

    // hasn't been inserted yet?  append to the list.
    sorted->append(toAdd);
}

/*! Sorts the given list of contacts \a cs according to the provided \a sortOrders */
QList<QContactLocalId> QContactManagerEngine::sortContacts(const QList<QContact>& cs, const QList<QContactSortOrder>& sortOrders)
{
    QList<QContactLocalId> sortedIds;
    QList<QContact> sortedContacts;
    if (!sortOrders.isEmpty()) {
        foreach (const QContact& c, cs) {
            QContactManagerEngine::addSorted(&sortedContacts, c, sortOrders);
        }

        foreach(const QContact& c, sortedContacts) {
            sortedIds.append(c.localId());
        }
    } else {
        foreach(const QContact& c, cs) {
            sortedIds.append(c.localId());
        }
    }
    return sortedIds;
}

/*!
  Notifies the manager engine that the given request \a req has been destroyed
 */
void QContactManagerEngine::requestDestroyed(QContactAbstractRequest* req)
{
    Q_UNUSED(req);
}

/*!
  Asks the manager engine to begin the given request \a req which
  is currently in a (re)startable state.
  Returns true if the request was started successfully, else returns false.

  \sa QContactAbstractRequest::start()
 */
bool QContactManagerEngine::startRequest(QContactAbstractRequest* req)
{
    Q_UNUSED(req);
    return false;
}

/*!
  Asks the manager engine to cancel the given request \a req which was
  previously started and is currently in a cancellable state.
  Returns true if cancellation of the request was started successfully,
  otherwise returns false.

  \sa startRequest(), QContactAbstractRequest::cancel()
 */
bool QContactManagerEngine::cancelRequest(QContactAbstractRequest* req)
{
    Q_UNUSED(req);
    return false;
}

/*!
  Blocks until the manager engine has completed the given request \a req
  which was previously started, or until \a msecs milliseconds have passed.
  Returns true if the request was completed, and false if the request was not in the
  \c QContactAbstractRequest::Active state or no progress could be reported.

  \sa startRequest()
 */
bool QContactManagerEngine::waitForRequestFinished(QContactAbstractRequest* req, int msecs)
{
    Q_UNUSED(req);
    Q_UNUSED(msecs);
    return false;
}

/*!
  Updates the given asynchronous request \a req by setting the new \a state
  of the request.  If the new state is different, the stateChanged() signal
  will be emitted by the request.
 */
void QContactManagerEngine::updateRequestState(QContactAbstractRequest* req, QContactAbstractRequest::State state)
{
    if (req->d_ptr->m_state != state) {
        req->d_ptr->m_state = state;
        emit req->stateChanged(state);
    }
}

/*!
  Updates the given QContactLocalIdFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactLocalIdFetchRequest(QContactLocalIdFetchRequest* req, const QList<QContactLocalId>& result, QContactManager::Error error, QContactAbstractRequest::State newState)
{
    QContactLocalIdFetchRequestPrivate* rd = static_cast<QContactLocalIdFetchRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_ids = result;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactFetchRequest(QContactFetchRequest* req, const QList<QContact>& result, QContactManager::Error error, QContactAbstractRequest::State newState)
{
    QContactFetchRequestPrivate* rd = static_cast<QContactFetchRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_contacts = result;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactRemoveRequest \a req with the operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactRemoveRequest(QContactRemoveRequest* req, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    QContactRemoveRequestPrivate* rd = static_cast<QContactRemoveRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_errors = errorMap;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactSaveRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateContactSaveRequest(QContactSaveRequest* req, const QList<QContact>& result, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    QContactSaveRequestPrivate* rd = static_cast<QContactSaveRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_errors = errorMap;
    rd->m_contacts = result;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactDetailDefinitionSaveRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateDefinitionSaveRequest(QContactDetailDefinitionSaveRequest* req, const QList<QContactDetailDefinition>& result, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    QContactDetailDefinitionSaveRequestPrivate* rd = static_cast<QContactDetailDefinitionSaveRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_errors = errorMap;
    rd->m_definitions = result;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactDetailDefinitionRemoveRequest \a req with the operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateDefinitionRemoveRequest(QContactDetailDefinitionRemoveRequest* req, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    QContactDetailDefinitionRemoveRequestPrivate* rd = static_cast<QContactDetailDefinitionRemoveRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_errors = errorMap;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactDetailDefinitionFetchRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateDefinitionFetchRequest(QContactDetailDefinitionFetchRequest* req, const QMap<QString, QContactDetailDefinition>& result, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    QContactDetailDefinitionFetchRequestPrivate* rd = static_cast<QContactDetailDefinitionFetchRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_errors = errorMap;
    rd->m_definitions = result;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactRelationshipSaveRequest \a req with the latest results \a result, operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateRelationshipSaveRequest(QContactRelationshipSaveRequest* req, const QList<QContactRelationship>& result, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    QContactRelationshipSaveRequestPrivate* rd = static_cast<QContactRelationshipSaveRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_errors = errorMap;
    rd->m_relationships = result;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactRelationshipRemoveRequest \a req with the operation error \a error, and map of input index to individual error \a errorMap.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateRelationshipRemoveRequest(QContactRelationshipRemoveRequest* req, QContactManager::Error error, const QMap<int, QContactManager::Error>& errorMap, QContactAbstractRequest::State newState)
{
    QContactRelationshipRemoveRequestPrivate* rd = static_cast<QContactRelationshipRemoveRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_errors = errorMap;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

/*!
  Updates the given QContactRelationshipFetchRequest \a req with the latest results \a result, and operation error \a error.
  In addition, the state of the request will be changed to \a newState.

  It then causes the request to emit its resultsAvailable() signal to notify clients of the request progress.

  If the new request state is different from the previous state, the stateChanged() signal will also be emitted from the request.
 */
void QContactManagerEngine::updateRelationshipFetchRequest(QContactRelationshipFetchRequest* req, const QList<QContactRelationship>& result, QContactManager::Error error, QContactAbstractRequest::State newState)
{
    QContactRelationshipFetchRequestPrivate* rd = static_cast<QContactRelationshipFetchRequestPrivate*>(req->d_ptr);
    req->d_ptr->m_error = error;
    rd->m_relationships = result;
    bool emitState = rd->m_state != newState;
    rd->m_state = newState;
    emit req->resultsAvailable();
    if (emitState)
        emit req->stateChanged(newState);
}

#include "moc_qcontactmanagerengine.cpp"

QTM_END_NAMESPACE
