#include "defines.h"
#include "ErrnoTypes.h"

QString getErrnoDescription(int err)
{
  switch (err)
  {
    case SUCCESS: return "";
    case WRONG_TOKEN_ERROR: return "Wrong authorization";
    case USER_ALREADY_EXIST_ERROR: return "User already exists";
    case USER_DOES_NOT_EXIST_ERROR: return "User does not exit";
    case CHANNEL_ALREADY_EXIST_ERROR: return "Channel already exists";
    case CHANNEL_DOES_NOT_EXIST_ERROR: return "Channel does not exit";
    case SUBSCRIPTION_ALREADY_EXIST: return "Already subscribed";
    case INTERNAL_DB_ERROR: return "Internal server error";
    //INCORRECT_QUERY_NAME_ERROR ,
    //INCORRECT_JSON_ERROR ,
    case INCORRECT_CREDENTIALS_ERROR: return "Wrong credentials";
    case CHANNEL_NOT_SUBCRIBED_ERROR: return "User is not subscribed to channel";
    //CHANNEL_ALREADY_SUBSCRIBED_ERROR ,
    //TAG_DOES_NOT_EXIST_ERROR ,
    //TAG_ALREADY_EXIST_ERROR ,
    //NULL_TIMESLOT_ERROR ,
    //UNKNOWN_ERROR,
    //TMP_USER_ALREADY_EXIST_ERROR,
    case NETWORK_ERROR: return "Network server error";
    default: return "Unknown error";
  }
}
