#include "utils/jsonUtils.hpp"

namespace	jsonUtils {

	bool	isErrorResponse(const Json::Value* p_res) {
		if (!p_res || !(*p_res))
			return true;

		return p_res->isMember("error");
	}
	
	const std::string	getResponseError(const Json::Value* p_res) {
		if (p_res && p_res->isMember("error") && (*p_res)["error"].isString())
			return (*p_res)["error"].asString();
		else
			return "Can't extract response error";
	}
}