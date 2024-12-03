#include "ResourceFormatter.hpp"
#include "Resource.hpp"
#include "ULIDFormatter.hpp"

fmt::appender fmt::formatter<brk::Resource>::format(
	const brk::Resource& res,
	format_context& ctx)
{
	return fmt::format_to(ctx.out(), "{} ({})", res.GetName(), res.GetId());
}
