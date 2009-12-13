/**************************************************
 * This code was created by Holger Schurig @ mn-logistik.de.
 * Released under LGPL 08.MAR.2001
 *
 * Contributions from...
 * -----------------------------------------------
 **************************************************/
#include <odbcinstext.h>

static const char *aHost[] =
{
	"localhost",
	NULL
};


int ODBCINSTGetProperties( HODBCINSTPROPERTY hLastProperty )
{
    hLastProperty->pNext                 = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
    hLastProperty                         = hLastProperty->pNext;
    memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
    hLastProperty->nPromptType            = ODBCINST_PROMPTTYPE_COMBOBOX;
    hLastProperty->aPromptData            = malloc( sizeof( aHost ) );
    memcpy( hLastProperty->aPromptData, aHost, sizeof( aHost ) );
    strncpy( hLastProperty->szName, "ServerNode", INI_MAX_PROPERTY_NAME );
    strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );

    hLastProperty->pNext                 = (HODBCINSTPROPERTY)malloc( sizeof(ODBCINSTPROPERTY) );
    hLastProperty                         = hLastProperty->pNext;
    memset( hLastProperty, 0, sizeof(ODBCINSTPROPERTY) );
    hLastProperty->nPromptType            = ODBCINST_PROMPTTYPE_TEXTEDIT;
    strncpy( hLastProperty->szName, "ServerDB", INI_MAX_PROPERTY_NAME );
    strncpy( hLastProperty->szValue, "", INI_MAX_PROPERTY_VALUE );

    return 1;
}
