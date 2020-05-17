from distutils.core import setup
import py2exe

setup(
    options={
        'py2exe': {
            'bundle_files': 1,
            'optimize': 2,
            'compressed': True,
            'packages': [
                wx
            ]
        }
    },
    zipfile=None,
    name='win-vind-GUI',
    version='1.0.0',
    windows=[{
        'script': 'test.py',
        'icon_resources': [(1, '../resources/icon512.ico')]
    }],
)