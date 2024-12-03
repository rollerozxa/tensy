package se.voxelmanip.tensy;

import org.libsdl.app.SDLActivity;

public class GameActivity extends SDLActivity {
	@Override
	protected String[] getLibraries() {
		return new String[] {
			"tensy"
		};
	}
}
