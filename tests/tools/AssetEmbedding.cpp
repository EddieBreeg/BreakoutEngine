#include <core/InputFile.hpp>
#include <core/StringView.hpp>
#include <tools/DataEmbed.hpp>
#include <fstream>
#include <sstream>

namespace {
	constexpr brk::StringView s_Lorem =
		R"(Lorem ipsum dolor sit amet, consectetur adipiscing elit. Ut odio sem, semper et ex eu, dapibus ultricies massa. Sed ac maximus turpis. Praesent tellus quam, congue eu velit eu, mattis bibendum nibh. Praesent nec felis ut eros ullamcorper tincidunt non vitae felis. Sed pharetra diam consequat, pretium metus vitae, gravida tellus. Aliquam bibendum tellus magna, vel tempus nisl ultricies at. Donec euismod eros rutrum leo fermentum semper. Morbi rhoncus libero erat, at lacinia nulla luctus id. Phasellus a odio vel elit egestas bibendum sed sit amet tortor.

Pellentesque placerat euismod vehicula. Etiam quam nulla, fermentum at aliquet in, pellentesque pellentesque urna. In venenatis dui sed quam tincidunt porta. Vivamus non felis ut neque facilisis elementum id eget velit. Sed a mauris vestibulum mauris semper commodo. Quisque bibendum felis mauris, vel laoreet ex blandit lacinia. Nulla rhoncus est at bibendum cursus. Fusce sed efficitur nisl.)";

	constexpr brk::StringView s_CompressedBase85 =
		R"(6<\%_0gSqh;cu2J1>)X-=kA0(&3pCH-Yi]HkPme4!\&;u)1tWhVl%V)Wd,0'OP5alC3_ZlSt]s(J-"pl(rB`q0Ld4JKY]+#&YVn>:K'`6UI,Xo*B]rUVpVmgZA7GI$kKqtegpLTbL2eH5D"lrE?"1aFGT"QE\e:D&e&Sd/LeD&`a$gsQ?IBcC@>cj#'D,ma%G2[<aO@Rk%A5`(cU:1Zt3Cn#,H'3$bG8t*b;tQ>&H[2Yg'P\8@5#5cWdJe>qB<H$7qnl#\M)h('Opk=TYKjkWD@M_*N`).K[CpEKW>H`VkB[Sh[/qJMDEH<n]6Te4s<m]aDe.::Q'!oIe,fJl0.$fsT7`"-V"T<\mTsC!Y5:%lGA=ct_s7+['@G<#qFDP#7l3kdM[j;IjC($nOdSjl9IYZt#DTlKp:Nguo]M`"Y^B=^VTR$D:j]3^c--12WGmK"%r3I;`gJ>$uPhP(s,e+[en7;k?EGq/ePZ-8kD`a1Z]#'?K__GY\X"7,s=rU3;Z>Bj\*iP`7N@^rA&@M_Pe%glVS'=HnqM,_@mHSRRBh+i$&LL2hRX)C;(UK!]0a)&X]1;&*J63r!N0V;#:/dcg4'?oUG^JqMas(!D5mGLG+oNd8-TKP6ZRWD^?SWJWNP@5=J],6<;5glY>G,S&pp[Qj)";
} // namespace

int main(int argc, char const* argv[])
{
	{
		brk::InputByteStream inStream{ {} };
		std::stringstream outStream;
		assert(!brk::tools::EmbedData(outStream, inStream, "asset"));
	}
	{
		std::vector<char> data(s_Lorem.begin(), s_Lorem.end());
		brk::InputByteStream inStream{ std::move(data) };
		std::stringstream outStream;
		assert(brk::tools::EmbedData(outStream, inStream, "lorem_ipsum"));
		std::string code = outStream.str();
		assert(brk::StringView{code}.Find(s_CompressedBase85) != brk::StringView::NPos);
		data = brk::tools::ReadEmbeddedData(
			s_CompressedBase85.GetPtr(),
			s_CompressedBase85.GetLen(),
			s_Lorem.GetLen());
		assert(data.size());
		const brk::StringView uncompressed{ data.data(), uint32(data.size()) };
		assert((uncompressed == s_Lorem));
	}
	return 0;
}
