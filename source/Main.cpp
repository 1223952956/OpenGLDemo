#include "ChessApplication.h"


int main(void)
{
	ApplicationSpecification spec;
	spec.Title = "Chess";
	spec.Width = 1920;
	spec.Height = 1080;

	ChessApplication app(spec);
	
	return app.Run();
}

