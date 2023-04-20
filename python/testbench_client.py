#!/usr/bin/env python3
import time

import numpy as np
import pyigtl
import pytest


@pytest.fixture
def igtclient():
    client = pyigtl.OpenIGTLinkClient(host='127.0.0.1', port=18944)
    time.sleep(1.0)
    return client


def test_transform(igtclient):
    success = 0
    input_message = igtclient.wait_for_message('transform', timeout=3)

    assert input_message is not None

    matrix = input_message.matrix
    assert np.all(matrix == np.eye(4)) == True

    output_message = pyigtl.TransformMessage(
        matrix,
        device_name='transform-modified'
    )
    status = igtclient.send_message(output_message)
    assert status == True

